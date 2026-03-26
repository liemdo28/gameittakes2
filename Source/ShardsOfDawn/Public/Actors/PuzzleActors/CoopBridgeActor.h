// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interface_Interactive.h"
#include "CoopBridgeActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USceneComponent;
class UMaterialInterface;
class USoundBase;

/**
 * ACoopBridgeActor — Visualizes co-op interdependence.
 *
 * Only fully descends (becomes walkable) when all required puzzle shards
 * are activated. Poll-based: reads shard count from ASODGameMode each tick.
 * Implements IInterface_Interactive for reset via GameMode.
 */
UCLASS(Blueprintable, BlueprintType)
class SHARDSOFDAWN_API ACoopBridgeActor : public AActor, public IInterface_Interactive
{
	GENERATED_BODY()

public:
	ACoopBridgeActor();

	// ── IInterface_Interactive ──────────────────────────────────────────────
	void ResetPuzzleState_Implementation() override;

	// ── Configuration ─────────────────────────────────────────────────────────
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Config",
		meta = (DisplayName = "Required Activation Count"))
	int32 RequiredActivationCount = 6;

	/** Shard IDs to monitor. All must be active for bridge to lower. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Config",
		meta = (DisplayName = "Monitored Shard IDs"))
	TArray<FName> MonitoredShardIDs;

	/** Seconds to wait after all shards activated before lowering. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Config",
		meta = (ClampMin = "0.0", ClampMax = "10.0", DisplayName = "Activation Delay (s)"))
	float ActivationDelay = 1.5f;

	// ── State ───────────────────────────────────────────────────────────────
	UPROPERTY(ReplicatedUsing = OnRep_IsLowered, BlueprintReadOnly,
		Category = "CoopBridge|State", Meta = (DisplayName = "Is Lowered"))
	bool bIsLowered = false;

	UPROPERTY(ReplicatedUsing = OnRep_ActivationProgress, BlueprintReadOnly,
		Category = "CoopBridge|State", Meta = (DisplayName = "Activation Progress (0-1)"))
	float ActivationProgress = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "CoopBridge", Meta = (DisplayName = "Get Current Activation Count"))
	int32 GetCurrentActivationCount() const;

	UFUNCTION(BlueprintPure, Category = "CoopBridge", Meta = (DisplayName = "Get Activation Progress"))
	float GetActivationProgress() const { return ActivationProgress; }

	UFUNCTION(BlueprintCallable, Category = "CoopBridge", Meta = (DisplayName = "Is Fully Activated"))
	bool IsFullyActivated() const;

	// ── Gameplay Tags ───────────────────────────────────────────────────────
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Tags")
	FGameplayTagContainer BridgeTagContainer;

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ── Components ───────────────────────────────────────────────────────────
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoopBridge|Components",
		Meta = (DisplayName = "Bridge Mesh"))
	TObjectPtr<UStaticMeshComponent> BridgeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoopBridge|Components",
		Meta = (DisplayName = "Proximity Zone"))
	TObjectPtr<UBoxComponent> ProximityZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoopBridge|Components",
		Meta = (DisplayName = "Start Location (Raised)"))
	TObjectPtr<USceneComponent> StartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoopBridge|Components",
		Meta = (DisplayName = "Lowered Location"))
	TObjectPtr<USceneComponent> LoweredLocation;

	// ── Visual Materials ────────────────────────────────────────────────────
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Visual",
		Meta = (DisplayName = "Locked Material"))
	TSoftObjectPtr<UMaterialInterface> LockedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Visual",
		Meta = (DisplayName = "Partially Active Material"))
	TSoftObjectPtr<UMaterialInterface> PartiallyActiveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Visual",
		Meta = (DisplayName = "Fully Active Material"))
	TSoftObjectPtr<UMaterialInterface> FullyActiveMaterial;

	// ── Audio ───────────────────────────────────────────────────────────────
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Audio",
		Meta = (DisplayName = "Lower Sound"))
	TSoftObjectPtr<USoundBase> LowerSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Audio",
		Meta = (DisplayName = "Raise Sound"))
	TSoftObjectPtr<USoundBase> RaiseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoopBridge|Audio",
		Meta = (DisplayName = "Activation Tick Sound"))
	TSoftObjectPtr<USoundBase> ActivationTickSound;

	// ── Rep Notify ──────────────────────────────────────────────────────────
	UFUNCTION()
	void OnRep_IsLowered();

	UFUNCTION()
	void OnRep_ActivationProgress();

	// ── Internal Logic ──────────────────────────────────────────────────────
	void UpdateActivationState();
	void TriggerLower();
	void TriggerRaise();
	void ApplyMaterial(UMaterialInterface* Mat);
	void ApplyBridgePose(bool bLowered);

	FTimerHandle LowerTimerHandle;
	int32 CachedActivationCount = 0;
};
