// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SODGameplayAbility.h"
#include "SODAbility_ShadowAnchor.generated.h"

/**
 * Shadow Anchor ability (Nam).
 * Locks a target actor in its current state, preventing physics/state changes.
 * Core mechanic: Nam anchors what Linh reveals -> puzzle solved.
 */
UCLASS()
class SHARDSOFDAWN_API USODAbility_ShadowAnchor : public USODGameplayAbility
{
	GENERATED_BODY()

public:
	USODAbility_ShadowAnchor();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "SOD|ShadowAnchor")
	float AnchorRange;

	UPROPERTY(EditDefaultsOnly, Category = "SOD|ShadowAnchor")
	float AnchorMaxDuration;

	UPROPERTY(EditDefaultsOnly, Category = "SOD|ShadowAnchor")
	TSubclassOf<class UGameplayEffect> AnchorEffect;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentTarget;
};
