// Copyright Shards of Dawn. All Rights Reserved.

#include "UI/SODHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

ASODHUD::ASODHUD()
{
}

void ASODHUD::DrawHUD()
{
	Super::DrawHUD();

	// Update ping markers
	for (int32 i = ActivePings.Num() - 1; i >= 0; --i)
	{
		ActivePings[i].RemainingTime -= GetWorld()->GetDeltaSeconds();
		if (ActivePings[i].RemainingTime <= 0.0f)
		{
			ActivePings.RemoveAt(i);
			continue;
		}

		// Project ping location to screen
		FVector2D ScreenPos;
		if (GetOwningPlayerController()->ProjectWorldLocationToScreen(ActivePings[i].Location, ScreenPos))
		{
			// Draw ping indicator
			DrawRect(FLinearColor::Yellow, ScreenPos.X - 5, ScreenPos.Y - 5, 10, 10);
		}
	}
}

void ASODHUD::ShowPingMarker(FVector WorldLocation, float Duration)
{
	FPingMarker NewPing;
	NewPing.Location = WorldLocation;
	NewPing.RemainingTime = Duration;
	ActivePings.Add(NewPing);
}

void ASODHUD::ShowCheckpointNotification()
{
	// Trigger notification widget (implemented in Blueprint)
}

void ASODHUD::UpdateAbilityCooldown(int32 AbilityIndex, float CooldownPercent)
{
	// Update cooldown display (implemented in Blueprint widget)
}
