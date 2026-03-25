// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SODHUD.generated.h"

/**
 * Base HUD class for Shards of Dawn.
 * Displays: co-op sync indicator, ability cooldowns, ping markers, checkpoint notifications.
 */
UCLASS()
class SHARDSOFDAWN_API ASODHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASODHUD();

	virtual void DrawHUD() override;

	/** Show ping marker at world location */
	UFUNCTION(BlueprintCallable, Category = "SOD|UI")
	void ShowPingMarker(FVector WorldLocation, float Duration);

	/** Show checkpoint notification */
	UFUNCTION(BlueprintCallable, Category = "SOD|UI")
	void ShowCheckpointNotification();

	/** Show ability cooldown indicator */
	UFUNCTION(BlueprintCallable, Category = "SOD|UI")
	void UpdateAbilityCooldown(int32 AbilityIndex, float CooldownPercent);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|UI")
	TSubclassOf<class UUserWidget> MainHUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|UI")
	TObjectPtr<UUserWidget> MainHUDWidget;

private:
	struct FPingMarker
	{
		FVector Location;
		float RemainingTime;
	};
	TArray<FPingMarker> ActivePings;
};
