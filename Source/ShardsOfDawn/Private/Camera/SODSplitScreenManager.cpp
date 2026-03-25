// Copyright Shards of Dawn. All Rights Reserved.

#include "Camera/SODSplitScreenManager.h"
#include "Characters/SODPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameViewportClient.h"

ASODSplitScreenManager::ASODSplitScreenManager()
	: MergeDistance(800.0f)
	, SplitDistance(1200.0f)
	, BlendSpeed(3.0f)
	, CurrentMode(ESODSplitScreenMode::Dynamic)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASODSplitScreenManager::BeginPlay()
{
	Super::BeginPlay();
}

void ASODSplitScreenManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentMode == ESODSplitScreenMode::Dynamic)
	{
		UpdateDynamicSplit();
	}
}

void ASODSplitScreenManager::UpdateDynamicSplit()
{
	float Distance = GetPlayerDistance();

	if (Distance < 0.0f)
	{
		return; // Not enough players
	}

	UGameViewportClient* Viewport = GetWorld()->GetGameViewport();
	if (!Viewport)
	{
		return;
	}

	if (Distance <= MergeDistance)
	{
		// Players are close: use merged camera
		// In merged mode, P2's viewport is hidden and P1's camera encompasses both
		Viewport->SetForceDisableSplitscreen(true);
	}
	else if (Distance >= SplitDistance)
	{
		// Players are far apart: full split-screen
		Viewport->SetForceDisableSplitscreen(false);
	}
	// Between MergeDistance and SplitDistance: interpolate (handled by viewport blend)
}

float ASODSplitScreenManager::GetPlayerDistance() const
{
	TArray<ASODPlayerCharacter*> Players = GetPlayers();
	if (Players.Num() < 2)
	{
		return -1.0f;
	}

	return FVector::Dist(Players[0]->GetActorLocation(), Players[1]->GetActorLocation());
}

TArray<ASODPlayerCharacter*> ASODSplitScreenManager::GetPlayers() const
{
	TArray<ASODPlayerCharacter*> Result;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			ASODPlayerCharacter* Char = Cast<ASODPlayerCharacter>(PC->GetPawn());
			if (Char)
			{
				Result.Add(Char);
			}
		}
	}

	return Result;
}

void ASODSplitScreenManager::SetSplitMode(ESODSplitScreenMode Mode)
{
	CurrentMode = Mode;

	UGameViewportClient* Viewport = GetWorld()->GetGameViewport();
	if (!Viewport)
	{
		return;
	}

	switch (Mode)
	{
	case ESODSplitScreenMode::Merged:
		Viewport->SetForceDisableSplitscreen(true);
		break;
	case ESODSplitScreenMode::SplitHorizontal:
	case ESODSplitScreenMode::SplitVertical:
		Viewport->SetForceDisableSplitscreen(false);
		break;
	case ESODSplitScreenMode::Dynamic:
		// Handled in tick
		break;
	}
}

void ASODSplitScreenManager::TriggerHeroShot(FVector FocusPoint, float Duration)
{
	// Temporarily merge cameras and focus on the key moment
	SetSplitMode(ESODSplitScreenMode::Merged);

	// Set timer to restore dynamic mode
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		SetSplitMode(ESODSplitScreenMode::Dynamic);
	}, Duration, false);
}
