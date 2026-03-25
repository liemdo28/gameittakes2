// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "Characters/SodPlayerCharacter.h"
#include "Interfaces/Interface_Interactive.h"
#include "GameFramework/Actor.h"
#include "SodPuzzleActorBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;
class URotatingMovementComponent;

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
    bool CanInteract_Implementation(ASodPlayerCharacter* Interactor) const override;
    virtual void OnInteract_Implementation(ASodPlayerCharacter* Interactor) override;

    // ── Puzzle Properties ────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    FText PuzzleName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle", meta = (ExposeOnSpawn = "true"))
    bool bIsActivated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    FName ShardID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    bool bRequiresBothArchetypes = false;

    /** Light Weaver / Shadow Walker / Either */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    TEnumAsByte<EPlayerArchetype> RequiredArchetype = EPlayerArchetype::Any;

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
    void BP_OnActivated(ASodPlayerCharacter* Activator);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Deactivated"))
    void BP_OnDeactivated(ASodPlayerCharacter* Deactivator);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Interaction Failed"))
    void BP_OnInteractionFailed(ASodPlayerCharacter* Interactor, const FText& Reason);

    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void ResetPuzzleState();

protected:
    UFUNCTION(Server, Reliable)
    void Server_SetActivated(bool bActivated, ASodPlayerCharacter* Interactor);
    void ApplyMaterial(UMaterialInterface* Material);
    void PlaySoundAtLocation(USoundBase* Sound);
    virtual void HandleActivationStateChanged(bool bActivated);
    bool HasBothArchetypesReady(ASodPlayerCharacter* Interactor) const;
    void ApplyActivationState(bool bActivated, ASodPlayerCharacter* Interactor);

    UPROPERTY(ReplicatedUsing = OnRep_IsActivated)
    bool RepIsActivated = false;

    UFUNCTION()
    void OnRep_IsActivated();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
