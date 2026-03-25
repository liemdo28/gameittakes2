// Copyright Shards of Dawn. All Rights Reserved.

#include "Abilities/SODAbility_LightPulse.h"
#include "Characters/SODPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

USODAbility_LightPulse::USODAbility_LightPulse()
	: PulseRadius(800.0f)
	, PulseDuration(3.0f)
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Primary")));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Light.Pulse")));

	CooldownGameplayEffectClass = nullptr; // Set in Blueprint
}

void USODAbility_LightPulse::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ASODPlayerCharacter* Character = GetSODCharacter();
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Perform sphere overlap to find light-reactive actors
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(PulseRadius);

	bool bHit = Character->GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Character->GetActorLocation(),
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere
	);

	if (bHit)
	{
		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* Actor = Overlap.GetActor())
			{
				// Apply reveal gameplay effect to reactive actors
				if (RevealEffect && Actor->FindComponentByClass<UAbilitySystemComponent>())
				{
					UAbilitySystemComponent* TargetASC = Actor->FindComponentByClass<UAbilitySystemComponent>();
					if (TargetASC)
					{
						FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(RevealEffect);
						TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
		}
	}

	// End after duration (using timer in a real implementation)
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void USODAbility_LightPulse::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
