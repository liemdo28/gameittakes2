// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interface_Interactive.h"
#include "GameFramework/Actor.h"
#include "SodPuzzleActorBase.generated.h"

class ASODPlayerCharacter;
class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;
class URotatingMovementComponent;

UENUM(BlueprintType)
enum class ESODPuzzleRoleRequirement : uint8
{
    Light UMETA(DisplayName = "Linh - Light"),
    Shadow UMETA(DisplayName = "Nam - Shadow"),
    Any UMETA(DisplayName = "Any")
};

/**
 * Base class for all puzzle objects in Shards of Dawn.
 * Provides interaction, archetype affinity, and visual feedback.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class SHARDSOFDAWN_API ASodPuzzleActorBase : public AActor, public IInterface_Interactive
{
    GENERATED_BODY()

public:
    ASodPuzzleActorBase();

    // ── IInterface_Interactive ──────────────────────────────────
    FText GetInteractionPrompt_Implementation() const override;
    bool CanInteract_Implementation(ASODPlayerCharacter* Interactor) const override;
    virtual void OnInteract_Implementation(ASODPlayerCharacter* Interactor) override;
    void ResetPuzzleState_Implementation() override;

    // ── Puzzle Properties ────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    FText PuzzleName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle", meta = (ExposeOnSpawn = "true"))
    bool bIsActivated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    FName ShardID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    bool bRequiresBothArchetypes = false;

    /** Which co-op role can activate this puzzle, if any. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    ESODPuzzleRoleRequirement RequiredRole = ESODPuzzleRoleRequirement::Any;

    // ── Components ───────────────────────────────────────────────
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USphereComponent> InteractionZone;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UNiagaraComponent> VFXComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<URotatingMovementComponent> RotateComp;

    // ── Materials ────────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UMaterialInterface> InactiveMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UMaterialInterface> ActiveMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UMaterialInterface> LockedMaterial;

    // ── Audio ───────────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> ActivationSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> InteractionAttemptSound;

    // ── Events ──────────────────────────────────────────────────
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Activated"))
    void BP_OnActivated(ASODPlayerCharacter* Activator);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Deactivated"))
    void BP_OnDeactivated(ASODPlayerCharacter* Deactivator);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Interaction Failed"))
    void BP_OnInteractionFailed(ASODPlayerCharacter* Interactor, const FText& Reason);

protected:
    UFUNCTION(Server, Reliable)
    void Server_SetActivated(bool bActivated, ASODPlayerCharacter* Interactor);
    void ApplyMaterial(UMaterialInterface* Material);
    void PlaySoundAtLocation(USoundBase* Sound);
    virtual void HandleActivationStateChanged(bool bActivated);
    bool HasBothRolesReady(ASODPlayerCharacter* Interactor) const;
    void ApplyActivationState(bool bActivated, ASODPlayerCharacter* Interactor);

    UPROPERTY(ReplicatedUsing = OnRep_IsActivated)
    bool RepIsActivated = false;

    UFUNCTION()
    void OnRep_IsActivated();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
