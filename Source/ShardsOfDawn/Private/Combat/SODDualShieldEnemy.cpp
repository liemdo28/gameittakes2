// Copyright Shards of Dawn. All Rights Reserved.

#include "Combat/SODDualShieldEnemy.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "Net/UnrealNetwork.h"

ASODDualShieldEnemy::ASODDualShieldEnemy()
	: LightShieldHP(100.0f)
	, ShadowShieldHP(100.0f)
	, MaxHealth(200.0f)
	, CurrentPhase(ESODShieldPhase::LightShield)
	, FinisherTimeWindow(2.0f)
	, FinisherInputCount(0)
{
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* ASODDualShieldEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASODDualShieldEnemy::BeginPlay()
{
	Super::BeginPlay();

	CurrentLightShieldHP = LightShieldHP;
	CurrentShadowShieldHP = ShadowShieldHP;
	CurrentHealth = MaxHealth;

	// Start AI behavior tree
	if (HasAuthority() && BehaviorTreeAsset)
	{
		if (AAIController* AIC = Cast<AAIController>(GetController()))
		{
			AIC->RunBehaviorTree(BehaviorTreeAsset);
		}
	}
}

void ASODDualShieldEnemy::DamageShield(ESODShieldPhase TargetShield, float Damage)
{
	if (!HasAuthority() || CurrentPhase == ESODShieldPhase::Defeated)
	{
		return;
	}

	switch (TargetShield)
	{
	case ESODShieldPhase::LightShield:
		// Shadow player breaks Light Shield
		if (CurrentPhase == ESODShieldPhase::LightShield)
		{
			CurrentLightShieldHP = FMath::Max(0.0f, CurrentLightShieldHP - Damage);
		}
		break;

	case ESODShieldPhase::ShadowShield:
		// Light player breaks Shadow Shield
		if (CurrentPhase == ESODShieldPhase::ShadowShield)
		{
			CurrentShadowShieldHP = FMath::Max(0.0f, CurrentShadowShieldHP - Damage);
		}
		break;

	case ESODShieldPhase::Vulnerable:
		// Both players can deal damage
		if (CurrentPhase == ESODShieldPhase::Vulnerable)
		{
			CurrentHealth = FMath::Max(0.0f, CurrentHealth - Damage);
			if (CurrentHealth <= 0.0f)
			{
				CurrentPhase = ESODShieldPhase::Defeated;
				OnShieldPhaseChanged.Broadcast(CurrentPhase);
			}
		}
		break;

	default:
		break;
	}

	CheckShieldState();
}

void ASODDualShieldEnemy::CheckShieldState()
{
	ESODShieldPhase OldPhase = CurrentPhase;

	if (CurrentPhase == ESODShieldPhase::LightShield && CurrentLightShieldHP <= 0.0f)
	{
		CurrentPhase = ESODShieldPhase::ShadowShield;
	}
	else if (CurrentPhase == ESODShieldPhase::ShadowShield && CurrentShadowShieldHP <= 0.0f)
	{
		CurrentPhase = ESODShieldPhase::Vulnerable;
	}

	if (OldPhase != CurrentPhase)
	{
		OnShieldPhaseChanged.Broadcast(CurrentPhase);
	}
}

void ASODDualShieldEnemy::ServerAttemptFinisher_Implementation(AController* Instigator)
{
	if (CurrentPhase != ESODShieldPhase::Vulnerable)
	{
		return;
	}

	FinisherInputCount++;

	if (FinisherInputCount == 1)
	{
		// First player pressed finisher -> start time window for second player
		GetWorldTimerManager().SetTimer(FinisherTimerHandle, this, &ASODDualShieldEnemy::ResetFinisher, FinisherTimeWindow, false);
	}
	else if (FinisherInputCount >= 2)
	{
		// Both players executed finisher in time -> defeat enemy
		GetWorldTimerManager().ClearTimer(FinisherTimerHandle);
		CurrentPhase = ESODShieldPhase::Defeated;
		CurrentHealth = 0.0f;
		OnShieldPhaseChanged.Broadcast(CurrentPhase);

		UE_LOG(LogTemp, Log, TEXT("SOD: Co-op finisher executed on %s!"), *GetName());
	}
}

void ASODDualShieldEnemy::DebugDefeat()
{
	CurrentPhase = ESODShieldPhase::Defeated;
	CurrentHealth = 0.0f;
	CurrentLightShieldHP = 0.0f;
	CurrentShadowShieldHP = 0.0f;
	OnShieldPhaseChanged.Broadcast(CurrentPhase);
	UE_LOG(LogTemp, Log, TEXT("SOD: %s debug defeated"), *GetName());
}

void ASODDualShieldEnemy::ResetFinisher()
{
	FinisherInputCount = 0;
}
