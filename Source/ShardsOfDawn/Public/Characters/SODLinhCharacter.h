// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SODPlayerCharacter.h"
#include "SODLinhCharacter.generated.h"

/**
 * Linh - "The Spark" (Player 1, Light role)
 * Abilities: Light Pulse, Prism Dash, Beam Link
 * Gameplay identity: Reveals hidden paths, illuminates shadow zones, triggers state changes.
 */
UCLASS()
class SHARDSOFDAWN_API ASODLinhCharacter : public ASODPlayerCharacter
{
	GENERATED_BODY()

public:
	ASODLinhCharacter();

	/** Activate Light Pulse: reveals hidden objects in radius */
	UFUNCTION(BlueprintCallable, Category = "SOD|Ability|Light")
	void ActivateLightPulse();

	/** Is the light pulse currently active? */
	UFUNCTION(BlueprintPure, Category = "SOD|Ability|Light")
	bool IsLightPulseActive() const { return bLightPulseActive; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Radius of the light pulse reveal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Ability|Light")
	float LightPulseRadius;

	/** Duration of the light pulse */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Ability|Light")
	float LightPulseDuration;

	/** Cooldown between pulses */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Ability|Light")
	float LightPulseCooldown;

private:
	bool bLightPulseActive;
	float LightPulseTimer;
	float LightPulseCooldownTimer;
};
