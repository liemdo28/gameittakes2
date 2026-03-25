// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "Actors/PuzzleActors/SodPuzzleActorBase.h"
#include "LightShardPuzzleActor.generated.h"

/**
 * LightShardPuzzleActor — A shard that can only be activated by Light Weaver.
 * Glows with golden energy and emits light to the world.
 * Part of the Chapter 1 "Garden of Echoes" introductory puzzle set.
 */
UCLASS(Blueprintable, BlueprintType)
class SHARDSOFDAWN_API ALightShardPuzzleActor : public ASodPuzzleActorBase
{
    GENERATED_BODY()

public:
    ALightShardPuzzleActor();

protected:
    void BeginPlay() override;
    void Tick(float DeltaSeconds) override;

    // ── Light Properties ─────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightShard|Light")
    float LightIntensity = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightShard|Light",
        meta = (UIMin = "100.0", UIMax = "2000.0", ClampMin = "100.0", ClampMax = "2000.0"))
    float LightRadius = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightShard|Light")
    FLinearColor LightColor = FLinearColor(1.0f, 0.85f, 0.4f, 1.0f);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightShard|Components")
    TObjectPtr<class UPointLightComponent> PointLight;

    // ── Activation Animation ─────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightShard|Animation")
    float FloatAmplitude = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightShard|Animation")
    float FloatSpeed = 2.0f;

    // ── Archetype Enforcement ────────────────────────────────────
    bool CanInteract_Implementation(ASodPlayerCharacter* Interactor) const override;
    void OnInteract_Implementation(ASodPlayerCharacter* Interactor) override;

private:
    float InitialZ;
    float TimeAccum = 0.0f;

    void HandleActivationStateChanged(bool bActivated) override;
};
