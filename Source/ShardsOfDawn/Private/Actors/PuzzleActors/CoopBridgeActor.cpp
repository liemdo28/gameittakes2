// Copyright Shards of Dawn. All Rights Reserved.

#include "Actors/PuzzleActors/CoopBridgeActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Core/SODGameMode.h"
#include "Interfaces/Interface_Interactive.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

ACoopBridgeActor::ACoopBridgeActor()
{
	bReplicates = true;
	SetReplicateMovement(false);

	// ── Root ─────────────────────────────────────────────────────
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// ── Bridge Mesh ──────────────────────────────────────────────
	BridgeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BridgeMesh"));
	BridgeMesh->SetupAttachment(RootComponent);
	BridgeMesh->SetCollisionProfileName(TEXT("BlockAll"));
	BridgeMesh->SetIsReplicated(true);

	// ── Proximity Zone ────────────────────────────────────────────
	ProximityZone = CreateDefaultSubobject<UBoxComponent>(TEXT("ProximityZone"));
	ProximityZone->SetupAttachment(RootComponent);
	ProximityZone->SetBoxExtent(FVector(150.0f, 400.0f, 100.0f));
	ProximityZone->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// ── Locations ─────────────────────────────────────────────────
	StartLocation = CreateDefaultSubobject<USceneComponent>(TEXT("StartLocation"));
	StartLocation->SetupAttachment(RootComponent);
	StartLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));

	LoweredLocation = CreateDefaultSubobject<USceneComponent>(TEXT("LoweredLocation"));
	LoweredLocation->SetupAttachment(RootComponent);
	LoweredLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Start raised
	BridgeMesh->SetRelativeLocation(StartLocation->GetRelativeLocation());

	PrimaryActorTick.bCanEverTick = true;

	// Defaults
	RequiredActivationCount = 6;
	ActivationDelay = 1.5f;
	ActivationProgress = 0.0f;
}

void ACoopBridgeActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACoopBridgeActor, bIsLowered);
	DOREPLIFETIME(ACoopBridgeActor, ActivationProgress);
}

void ACoopBridgeActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PrimaryActorTick.SetTickFunctionEnable(true);
	}

	// Apply initial locked material
	ApplyMaterial(LockedMaterial.IsNull() ? nullptr : LockedMaterial.LoadSynchronous());
	ApplyBridgePose(bIsLowered);
}

void ACoopBridgeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasAuthority()) return;

	// Poll activation count from ASODGameMode (single source of truth for shard state)
	if (AGameMode* GM = UGameplayStatics::GetGameMode(this))
	{
		if (ASODGameMode* SODGM = Cast<ASODGameMode>(GM))
		{
			CachedActivationCount = 0;
			TArray<FName> ActiveIDs = SODGM->GetActivatedShardIDs();
			for (const FName& SID : MonitoredShardIDs)
			{
				if (ActiveIDs.Contains(SID))
				{
					CachedActivationCount++;
				}
			}

			float NewProgress = RequiredActivationCount > 0
				? static_cast<float>(CachedActivationCount) / static_cast<float>(RequiredActivationCount)
				: 0.0f;

			if (!FMath::IsNearlyEqual(NewProgress, ActivationProgress))
			{
				ActivationProgress = FMath::Clamp(NewProgress, 0.0f, 1.0f);
			}

			UpdateActivationState();
		}
	}
}

void ACoopBridgeActor::UpdateActivationState()
{
	if (CachedActivationCount >= RequiredActivationCount && !bIsLowered)
	{
		if (!GetWorldTimerManager().IsTimerActive(LowerTimerHandle))
		{
			GetWorldTimerManager().SetTimer(
				LowerTimerHandle, this, &ACoopBridgeActor::TriggerLower,
				ActivationDelay, false);
		}
	}
	else if (CachedActivationCount < RequiredActivationCount)
	{
		GetWorldTimerManager().ClearTimer(LowerTimerHandle);

		if (bIsLowered)
		{
			TriggerRaise();
		}
	}
}

void ACoopBridgeActor::TriggerLower()
{
	if (bIsLowered) return;

	bIsLowered = true;
	ApplyBridgePose(true);
	ApplyMaterial(FullyActiveMaterial.IsNull() ? nullptr : FullyActiveMaterial.LoadSynchronous());

	if (!LowerSound.IsNull())
	{
		UGameplayStatics::PlaySoundAtLocation(this, LowerSound.LoadSynchronous(), GetActorLocation());
	}
}

void ACoopBridgeActor::TriggerRaise()
{
	if (!bIsLowered) return;

	bIsLowered = false;
	GetWorldTimerManager().ClearTimer(LowerTimerHandle);
	ApplyBridgePose(false);
	ApplyMaterial(LockedMaterial.IsNull() ? nullptr : LockedMaterial.LoadSynchronous());

	if (!RaiseSound.IsNull())
	{
		UGameplayStatics::PlaySoundAtLocation(this, RaiseSound.LoadSynchronous(), GetActorLocation());
	}
}

void ACoopBridgeActor::OnRep_IsLowered()
{
	ApplyBridgePose(bIsLowered);
}

void ACoopBridgeActor::OnRep_ActivationProgress()
{
	if (ActivationProgress >= 1.0f)
	{
		ApplyMaterial(FullyActiveMaterial.IsNull() ? nullptr : FullyActiveMaterial.LoadSynchronous());
	}
	else if (ActivationProgress > 0.0f)
	{
		ApplyMaterial(PartiallyActiveMaterial.IsNull() ? nullptr : PartiallyActiveMaterial.LoadSynchronous());
	}
	else
	{
		ApplyMaterial(LockedMaterial.IsNull() ? nullptr : LockedMaterial.LoadSynchronous());
	}
}

bool ACoopBridgeActor::IsFullyActivated() const
{
	return CachedActivationCount >= RequiredActivationCount;
}

int32 ACoopBridgeActor::GetCurrentActivationCount() const
{
	return CachedActivationCount;
}

void ACoopBridgeActor::ApplyMaterial(UMaterialInterface* Mat)
{
	if (Mat && BridgeMesh)
	{
		BridgeMesh->SetMaterial(0, Mat);
	}
}

void ACoopBridgeActor::ApplyBridgePose(bool bLowered)
{
	if (!BridgeMesh || !StartLocation || !LoweredLocation) return;

	const FVector TargetLocation = bLowered
		? LoweredLocation->GetRelativeLocation()
		: StartLocation->GetRelativeLocation();

	BridgeMesh->SetRelativeLocation(TargetLocation);
}

void ACoopBridgeActor::ResetPuzzleState_Implementation()
{
	if (bIsLowered)
	{
		TriggerRaise();
	}
	ActivationProgress = 0.0f;
	CachedActivationCount = 0;
	ApplyMaterial(LockedMaterial.IsNull() ? nullptr : LockedMaterial.LoadSynchronous());
}
