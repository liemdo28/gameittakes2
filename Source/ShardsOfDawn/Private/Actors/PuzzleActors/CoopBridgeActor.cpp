// Copyright Shards of Dawn Team 2026

#include "Actors/PuzzleActors/CoopBridgeActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameModes/SodGameMode.h"
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
    SetActorRelativeLocation(FVector(0.0f, 0.0f, 300.0f));

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

    // Register with game mode's shard tracker
    if (HasAuthority())
    {
        if (AGameMode* GM = UGameplayStatics::GetGameMode(this))
        {
            if (ASodGameMode* SodGM = Cast<ASodGameMode>(GM))
            {
                // Game mode tracks all shard IDs; we poll it
            }
        }

        // Start tick to monitor shard state
        PrimaryActorTick.SetTickFunctionEnable(true);
    }

    // Apply initial locked material
    ApplyMaterial(LockedMaterial.Get());
}

void ACoopBridgeActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!HasAuthority()) return;

    // Poll activation progress from game mode
    if (AGameMode* GM = UGameplayStatics::GetGameMode(this))
    {
        if (ASodGameMode* SodGM = Cast<ASodGameMode>(GM))
        {
            CachedActivationCount = 0;
            for (const FName& SID : MonitoredShardIDs)
            {
                if (SodGM->ActivatedShardIDs.Contains(SID))
                {
                    CachedActivationCount++;
                }
            }

            float NewProgress = RequiredActivationCount > 0
                ? float(CachedActivationCount) / float(RequiredActivationCount)
                : 0.0f;

            if (!FMath::IsNearlyEqual(NewProgress, ActivationProgress))
            {
                ActivationProgress = FMath::Clamp(NewProgress, 0.0f, 1.0f);
                OnRep_ActivationProgress();
            }

            UpdateActivationState();
        }
    }
}

void ACoopBridgeActor::UpdateActivationState()
{
    if (CachedActivationCount >= RequiredActivationCount && !bIsLowered)
    {
        // All shards active — trigger lowering after delay
        GetWorldTimerManager().SetTimer(
            LowerTimerHandle, this, &ACoopBridgeActor::TriggerLower,
            ActivationDelay, false);
    }
    else if (CachedActivationCount < RequiredActivationCount && bIsLowered)
    {
        // Shards deactivated — raise bridge
        TriggerRaise();
    }
}

void ACoopBridgeActor::TriggerLower()
{
    if (bIsLowered) return;

    bIsLowered = true;

    // Animate bridge down
    if (LoweredLocation)
    {
        FVector Target = LoweredLocation->GetComponentLocation();
        SetActorLocation(Target, true);
    }

    ApplyMaterial(FullyActiveMaterial.Get());
    UGameplayStatics::PlaySoundAtLocation(this, LowerSound, GetActorLocation());
}

void ACoopBridgeActor::TriggerRaise()
{
    if (!bIsLowered) return;

    bIsLowered = false;
    GetWorldTimerManager().ClearTimer(LowerTimerHandle);

    if (StartLocation)
    {
        FVector Target = StartLocation->GetComponentLocation();
        SetActorLocation(Target, true);
    }

    ApplyMaterial(LockedMaterial.Get());
    UGameplayStatics::PlaySoundAtLocation(this, RaiseSound, GetActorLocation());
}

void ACoopBridgeActor::OnRep_IsLowered()
{
    // Client visual update handled by SetActorLocation in TriggerLower/Raise
}

void ACoopBridgeActor::OnRep_ActivationProgress()
{
    // Update material to reflect partial progress
    if (ActivationProgress >= 1.0f)
    {
        ApplyMaterial(FullyActiveMaterial.Get());
    }
    else if (ActivationProgress > 0.0f)
    {
        ApplyMaterial(PartiallyActiveMaterial.Get());
    }
    else
    {
        ApplyMaterial(LockedMaterial.Get());
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
