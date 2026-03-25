// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SODDualShieldEnemy.generated.h"

class UAbilitySystemComponent;
class UBehaviorTree;

/** Which shield layer is currently active */
UENUM(BlueprintType)
enum class ESODShieldPhase : uint8
{
	LightShield UMETA(DisplayName = "Light Shield - Break with Shadow"),
	ShadowShield UMETA(DisplayName = "Shadow Shield - Break with Light"),
	Vulnerable UMETA(DisplayName = "Vulnerable - Both can damage"),
	Defeated UMETA(DisplayName = "Defeated")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldPhaseChanged, ESODShieldPhase, NewPhase);

/**
 * Enemy with dual shield system requiring both players to defeat.
 * Light Shield can only be broken by Shadow player.
 * Shadow Shield can only be broken by Light player.
 * When both shields are broken -> Vulnerable -> co-op finisher opportunity.
 */
UCLASS()
class SHARDSOFDAWN_API ASODDualShieldEnemy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASODDualShieldEnemy();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Apply damage to a specific shield layer */
	UFUNCTION(BlueprintCallable, Category = "SOD|Combat")
	void DamageShield(ESODShieldPhase TargetShield, float Damage);

	/** Get current shield phase */
	UFUNCTION(BlueprintPure, Category = "SOD|Combat")
	ESODShieldPhase GetCurrentPhase() const { return CurrentPhase; }

	/** Is the enemy ready for a co-op finisher? */
	UFUNCTION(BlueprintPure, Category = "SOD|Combat")
	bool IsVulnerable() const { return CurrentPhase == ESODShieldPhase::Vulnerable; }

	/** Execute co-op finisher (both players must trigger simultaneously) */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "SOD|Combat")
	void ServerAttemptFinisher(AController* Instigator);

	UPROPERTY(BlueprintAssignable)
	FOnShieldPhaseChanged OnShieldPhaseChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Combat")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Combat")
	float LightShieldHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Combat")
	float ShadowShieldHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Combat")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|Combat")
	float CurrentLightShieldHP;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|Combat")
	float CurrentShadowShieldHP;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|Combat")
	float CurrentHealth;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "SOD|Combat")
	ESODShieldPhase CurrentPhase;

	/** Time window for both players to execute finisher */
	UPROPERTY(EditDefaultsOnly, Category = "SOD|Combat")
	float FinisherTimeWindow;

private:
	int32 FinisherInputCount;
	FTimerHandle FinisherTimerHandle;

	void ResetFinisher();
	void CheckShieldState();
};
