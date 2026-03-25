// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "Actors/PuzzleActors/SodPuzzleActorBase.h"
#include "ShadowShardPuzzleActor.generated.h"

/**
 * ShadowShardPuzzleActor — A shard that can only be activated by Shadow Walker.
 * Pulses with violet void energy.
 * Part of the Chapter 1 "Garden of Echoes" introductory puzzle set.
 */
UCLASS(Blueprintable, BlueprintType)
class SHARDSOFDAWN_API AShadowShardPuzzleActor : public ASodPuzzleActorBase
{
    GENERATED_BODY()

public:
    AShadowShardPuzzleActor();

protected:
    void BeginPlay() override;
    void Tick(float DeltaSeconds) override;

    // ── Shadow Properties ────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowShard|Light")
    float LightIntensity = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowShard|Light")
    float LightRadius = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowShard|Light")
    FLinearColor LightColor = FLinearColor(0.3f, 0.1f, 0.6f, 1.0f);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShadowShard|Components")
    TObjectPtr<class UPointLightComponent> VoidLight;

    // ── Void Animation ──────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowShard|Animation")
    float PulseAmplitude = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowShard|Animation")
    float PulseSpeed = 3.0f;

    // ── Archetype Enforcement ───────────────────────────────────
    bool CanInteract_Implementation(ASodPlayerCharacter* Interactor) const override;
    void OnInteract_Implementation(ASodPlayerCharacter* Interactor) override;

private:
    float InitialZ;
    float TimeAccum = 0.0f;

    void HandleActivationStateChanged(bool bActivated) override;
};
