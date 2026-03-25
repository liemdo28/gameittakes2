// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SODGameplayAbility.h"
#include "SODAbility_LightPulse.generated.h"

/**
 * Light Pulse ability (Linh).
 * Sends out a radial pulse that reveals hidden objects and changes environment state.
 * Core mechanic: Linh reveals -> creates puzzle opportunity -> Nam anchors.
 */
UCLASS()
class SHARDSOFDAWN_API USODAbility_LightPulse : public USODGameplayAbility
{
	GENERATED_BODY()

public:
	USODAbility_LightPulse();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "SOD|LightPulse")
	float PulseRadius;

	UPROPERTY(EditDefaultsOnly, Category = "SOD|LightPulse")
	float PulseDuration;

	UPROPERTY(EditDefaultsOnly, Category = "SOD|LightPulse")
	TSubclassOf<class UGameplayEffect> RevealEffect;
};
