// Copyright Shards of Dawn. All Rights Reserved.

#include "Puzzle/SODPuzzleActor.h"
#include "Characters/SODPlayerCharacter.h"
#include "Core/SODGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

ASODPuzzleActor::ASODPuzzleActor()
	: CurrentState(ESODPuzzleState::Neutral)
	, RevealTimeWindow(5.0f)
	, bResettable(true)
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	PuzzleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PuzzleMesh"));
	PuzzleMesh->SetupAttachment(RootScene);
	PuzzleMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ASODPuzzleActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASODPuzzleActor::OnLightInteract(ASODPlayerCharacter* LightPlayer)
{
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentState != ESODPuzzleState::Neutral)
	{
		return;
	}

	// Only Light player can reveal
	if (!LightPlayer || LightPlayer->GetPlayerRole() != ESODPlayerRole::Light)
	{
		return;
	}

	SetPuzzleState(ESODPuzzleState::LightRevealed);
	OnLightRevealed();

	// Start timer: if Shadow doesn't anchor in time, reset
	GetWorldTimerManager().SetTimer(RevealTimerHandle, this, &ASODPuzzleActor::ResetPuzzle, RevealTimeWindow, false);
}

void ASODPuzzleActor::OnShadowInteract(ASODPlayerCharacter* ShadowPlayer)
{
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentState != ESODPuzzleState::LightRevealed)
	{
		return;
	}

	// Only Shadow player can anchor
	if (!ShadowPlayer || ShadowPlayer->GetPlayerRole() != ESODPlayerRole::Shadow)
	{
		return;
	}

	// Clear reveal timer
	GetWorldTimerManager().ClearTimer(RevealTimerHandle);

	SetPuzzleState(ESODPuzzleState::ShadowAnchored);
	OnShadowLocked();

	// Immediately transition to solved
	SetPuzzleState(ESODPuzzleState::Solved);
	OnPuzzleSolved();

	// Reward co-op sync score
	USODGameInstance* GI = Cast<USODGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->AddCoOpSyncScore(0.05f);
	}
}

void ASODPuzzleActor::ResetPuzzle()
{
	if (!HasAuthority())
	{
		return;
	}

	if (CurrentState == ESODPuzzleState::Solved && !bResettable)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(RevealTimerHandle);
	SetPuzzleState(ESODPuzzleState::Neutral);
}

void ASODPuzzleActor::SetPuzzleState(ESODPuzzleState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		OnPuzzleStateChanged.Broadcast(NewState);
	}
}

void ASODPuzzleActor::OnRep_CurrentState()
{
	OnPuzzleStateChanged.Broadcast(CurrentState);
}

void ASODPuzzleActor::OnPuzzleSolved_Implementation()
{
	// Override in Blueprint: open door, move platform, trigger cinematic, etc.
	UE_LOG(LogTemp, Log, TEXT("SOD: Puzzle %s solved!"), *GetName());
}

void ASODPuzzleActor::OnLightRevealed_Implementation()
{
	// Override in Blueprint: show VFX, change material, etc.
}

void ASODPuzzleActor::OnShadowLocked_Implementation()
{
	// Override in Blueprint: lock VFX, shadow particles, etc.
}

void ASODPuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASODPuzzleActor, CurrentState);
}
