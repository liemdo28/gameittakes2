// Copyright Shards of Dawn. All Rights Reserved.

#include "Abilities/SODAbility_ShadowAnchor.h"
#include "Characters/SODPlayerCharacter.h"
#include "AbilitySystemComponent.h"

USODAbility_ShadowAnchor::USODAbility_ShadowAnchor()
	: AnchorRange(1200.0f)
	, AnchorMaxDuration(8.0f)
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Primary")));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Shadow.Anchor")));

	// This ability is more effective when partner has revealed something
	bRequiresPartnerState = false; // Can be used standalone, but co-op gives bonus
}

void USODAbility_ShadowAnchor::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	// Line trace to find anchor target
	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (!PC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector CameraLoc;
	FRotator CameraRot;
	PC->GetPlayerViewPoint(CameraLoc, CameraRot);

	FVector TraceEnd = CameraLoc + (CameraRot.Vector() * AnchorRange);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);

	bool bHit = Character->GetWorld()->LineTraceSingleByChannel(
		HitResult, CameraLoc, TraceEnd, ECC_GameTraceChannel2, QueryParams);

	if (bHit && HitResult.GetActor())
	{
		CurrentTarget = HitResult.GetActor();

		// Freeze the target in place
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(CurrentTarget->GetRootComponent()))
		{
			Prim->SetSimulatePhysics(false);
		}

		// Apply anchor gameplay effect if target has ASC
		if (AnchorEffect)
		{
			UAbilitySystemComponent* TargetASC = CurrentTarget->FindComponentByClass<UAbilitySystemComponent>();
			if (TargetASC)
			{
				FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(AnchorEffect);
				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void USODAbility_ShadowAnchor::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Release the anchor
	if (CurrentTarget.IsValid())
	{
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(CurrentTarget->GetRootComponent()))
		{
			Prim->SetSimulatePhysics(true);
		}
		CurrentTarget.Reset();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
