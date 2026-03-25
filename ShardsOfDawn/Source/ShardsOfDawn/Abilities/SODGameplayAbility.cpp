// Copyright Shards of Dawn. All Rights Reserved.

#include "Abilities/SODGameplayAbility.h"
#include "Characters/SODPlayerCharacter.h"
#include "AbilitySystemComponent.h"

USODGameplayAbility::USODGameplayAbility()
	: bRequiresPartnerState(false)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool USODGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// Co-op validation: check if partner has required state
	if (bRequiresPartnerState)
	{
		ASODPlayerCharacter* Owner = Cast<ASODPlayerCharacter>(ActorInfo->AvatarActor.Get());
		if (!Owner)
		{
			return false;
		}

		ASODPlayerCharacter* Partner = Owner->GetPartner();
		if (!Partner)
		{
			return false;
		}

		UAbilitySystemComponent* PartnerASC = Partner->GetAbilitySystemComponent();
		if (!PartnerASC || !PartnerASC->HasAllMatchingGameplayTags(RequiredPartnerTags))
		{
			return false;
		}
	}

	return true;
}

ASODPlayerCharacter* USODGameplayAbility::GetSODCharacter() const
{
	return Cast<ASODPlayerCharacter>(GetAvatarActorFromActorInfo());
}
