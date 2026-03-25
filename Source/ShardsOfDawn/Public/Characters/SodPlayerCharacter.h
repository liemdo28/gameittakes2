// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "SodPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;

/**
 * Enum representing the two archetypes: Light Weaver and Shadow Walker
 */
UENUM(BlueprintType)
enum class EPlayerArchetype : uint8
{
    LightWeaver   UMETA(DisplayName = "Light Weaver"),
    ShadowWalker  UMETA(DisplayName = "Shadow Walker")
};

/**
 * Base Player Character for Shards of Dawn
 * Handles movement, interaction, archetype-specific abilities, and co-op synchronization
 */
UCLASS(Blueprintable, BlueprintType)
class SHARDSOFDAWN_API ASodPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASodPlayerCharacter();

    // ── Archetype ──────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Archetype")
    EPlayerArchetype Archetype = EPlayerArchetype::LightWeaver;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Archetype")
    FText CharacterName;

    // ── Camera ──────────────────────────────────────────────────
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    // ── Visual ──────────────────────────────────────────────────
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    TObjectPtr<UGroomComponent> HairComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Mesh")
    TSoftObjectPtr<USkeletalMesh> CharacterMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Material")
    TSoftObjectPtr<UMaterialInterface> ArchetypeMaterial;

    // ── Movement ────────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BaseMoveSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BaseJumpVelocity = 700.0f;

    // ── Interaction ─────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
    float InteractionReach = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TEnumAsByte<ECollisionChannel> InteractionTraceChannel = ECC_GameTraceChannel1;

    // ── Net Role ────────────────────────────────────────────────
    UFUNCTION(BlueprintCallable, Category = "Network")
    FString GetDebugRoleString() const;

protected:
    // ── Input Bindings ──────────────────────────────────────────
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);
    void Jump();
    void StopJump();
    void TryInteract();

    // ── Gameplay Ability System ─────────────────────────────────
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
    TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

    // ── Interaction helpers ─────────────────────────────────────
    bool TraceForInteractable(FHitResult& OutHit) const;

    // ── Replicated Properties ───────────────────────────────────
    UPROPERTY(ReplicatedUsing = OnRep_ArchetypeChanged)
    EPlayerArchetype RepArchetype;

    UFUNCTION()
    void OnRep_ArchetypeChanged();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ── Animation ───────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TObjectPtr<UAnimMontage> InteractionMontage;

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Interact"))
    void BP_OnInteract(AActor* InteractableActor);
};
