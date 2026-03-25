// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "CoopBridgeActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USceneComponent;
class UMaterialInterface;
class USoundBase;

/**
 * CoopBridgeActor — Visualizes co-op interdependence.
 * Only fully descends (becomes walkable) when all required puzzle shards are activated.
 * Both players must stand on it together to proceed.
 */
UCLASS(Blueprintable, BlueprintType)
class SHARDSOFDAWN_API ACoopBridgeActor : public AActor
{
    GENERATED_BODY()

public:
    ACoopBridgeActor();

    // ── Configuration ────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Config")
    int32 RequiredActivationCount = 6;

    /** Shard IDs to monitor. All must be active for bridge to lower. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Config")
    TArray<FName> MonitoredShardIDs;

    /** Seconds to wait after all shards activated before lowering */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Config",
        meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float ActivationDelay = 1.5f;

    // ── State ────────────────────────────────────────────────────
    UPROPERTY(ReplicatedUsing = OnRep_IsLowered, BlueprintReadOnly, Category = "CoopBridge|State")
    bool bIsLowered = false;

    UPROPERTY(ReplicatedUsing = OnRep_ActivationProgress, BlueprintReadOnly, Category = "CoopBridge|State")
    float ActivationProgress = 0.0f;

    UFUNCTION(BlueprintCallable, Category = "CoopBridge")
    int32 GetCurrentActivationCount() const;

    UFUNCTION(BlueprintCallable, Category = "CoopBridge")
    float GetActivationProgress() const { return ActivationProgress; }

    UFUNCTION(BlueprintCallable, Category = "CoopBridge")
    bool IsFullyActivated() const;

    // ── Gameplay Tags ────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Tags")
    FGameplayTagContainer BridgeTagContainer;

protected:
    void BeginPlay() override;
    void Tick(float DeltaSeconds) override;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ── Components ──────────────────────────────────────────────
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoopBridge|Components")
    TObjectPtr<UStaticMeshComponent> BridgeMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoopBridge|Components")
    TObjectPtr<UBoxComponent> ProximityZone;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoopBridge|Components")
    TObjectPtr<USceneComponent> StartLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoopBridge|Components")
    TObjectPtr<USceneComponent> LoweredLocation;

    // ── Visual Materials ─────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Visual")
    TSoftObjectPtr<UMaterialInterface> LockedMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Visual")
    TSoftObjectPtr<UMaterialInterface> PartiallyActiveMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Visual")
    TSoftObjectPtr<UMaterialInterface> FullyActiveMaterial;

    // ── Audio ───────────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Audio")
    TSoftObjectPtr<USoundBase> LowerSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Audio")
    TSoftObjectPtr<USoundBase> RaiseSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Audio")
    TSoftObjectPtr<USoundBase> ActivationTickSound;

    // ── Rep Notify ──────────────────────────────────────────────
    UFUNCTION()
    void OnRep_IsLowered();

    UFUNCTION()
    void OnRep_ActivationProgress();

    // ── Internal Logic ───────────────────────────────────────────
    void UpdateActivationState();
    void TriggerLower();
    void TriggerRaise();
    void ApplyMaterial(UMaterialInterface* Mat);
    void ApplyBridgePose(bool bLowered);

    // Timer handles
    FTimerHandle LowerTimerHandle;

    // Cached count
    int32 CachedActivationCount = 0;
};
