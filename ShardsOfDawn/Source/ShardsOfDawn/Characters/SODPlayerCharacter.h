// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "SODPlayerCharacter.generated.h"

class UAbilitySystemComponent;
class USODAbilitySet;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/** Player role in the co-op pair */
UENUM(BlueprintType)
enum class ESODPlayerRole : uint8
{
	Light UMETA(DisplayName = "Linh - Light"),
	Shadow UMETA(DisplayName = "Nam - Shadow")
};

/**
 * Base player character for Shards of Dawn.
 * Both Linh (Light) and Nam (Shadow) derive from this.
 * Integrates with GAS for ability management.
 */
UCLASS()
class SHARDSOFDAWN_API ASODPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASODPlayerCharacter();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Get this character's role */
	UFUNCTION(BlueprintPure, Category = "SOD|Character")
	ESODPlayerRole GetPlayerRole() const { return PlayerRole; }

	/** Get the partner character in co-op */
	UFUNCTION(BlueprintPure, Category = "SOD|CoOp")
	ASODPlayerCharacter* GetPartner() const;

	/** Ping system: signal partner about a point of interest */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "SOD|CoOp")
	void ServerPing(FVector PingLocation);

	/** Called when partner pings */
	UFUNCTION(BlueprintImplementableEvent, Category = "SOD|CoOp")
	void OnPartnerPinged(FVector PingLocation);

	/** Shared revive: revive downed partner */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "SOD|CoOp")
	void ServerRevivePartner();

	UFUNCTION(BlueprintPure, Category = "SOD|Character")
	bool IsDown() const { return bIsDown; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Movement input handlers */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpPressed();
	void InteractPressed();
	void AbilityPrimaryPressed();
	void AbilitySecondaryPressed();
	void PingPressed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SOD|Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Character")
	ESODPlayerRole PlayerRole;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Input")
	TObjectPtr<UInputAction> AbilityPrimaryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Input")
	TObjectPtr<UInputAction> AbilitySecondaryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Input")
	TObjectPtr<UInputAction> PingAction;

	/** Gameplay tags granted to this character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Abilities")
	FGameplayTagContainer StartingTags;

	/** Abilities granted on spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> StartingAbilities;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "SOD|Character")
	bool bIsDown;
};
