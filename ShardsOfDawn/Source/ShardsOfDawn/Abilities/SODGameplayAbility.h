// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SODGameplayAbility.generated.h"

class ASODPlayerCharacter;

/**
 * Base gameplay ability for Shards of Dawn.
 * Extends GAS with co-op validation (some abilities require partner state).
 */
UCLASS()
class SHARDSOFDAWN_API USODGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USODGameplayAbility();

	/** Does this ability require the partner to be in a specific state? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|CoOp")
	bool bRequiresPartnerState;

	/** Tag the partner must have for this ability to activate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|CoOp", meta = (EditCondition = "bRequiresPartnerState"))
	FGameplayTagContainer RequiredPartnerTags;

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;

	/** Get the owning SOD character */
	UFUNCTION(BlueprintPure, Category = "SOD|Ability")
	ASODPlayerCharacter* GetSODCharacter() const;
};
