// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SODGameMode.generated.h"

class ASODPlayerCharacter;
class ASodPlayerController;

/**
 * ASODGameMode — CANONICAL game mode for Shards of Dawn.
 *
 * Responsibilities:
 * - Spawns Linh (Light) and Nam (Shadow) based on player index
 * - Manages cooperative puzzle state via ActivatedShardIDs
 * - Provides checkpoint / respawn system
 * - Replicates co-op session state to all clients
 *
 * Architecture note: This is the SINGLE authoritative GameMode class.
 * Former duplicate classes (GameModes/SodGameMode) have been merged here.
 */
UCLASS()
class SHARDSOFDAWN_API ASODGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASODGameMode();

	// ── Player Session ──────────────────────────────────────────────────────────
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	// ── Pawn Selection ────────────────────────────────────────────────────────
	/** Returns the appropriate player-start tag for Linh (player 1 / Light) */
	static constexpr FName LINH_START_TAG = FName(TEXT("PlayerStart.Light"));
	/** Returns the appropriate player-start tag for Nam (player 2 / Shadow) */
	static constexpr FName NAM_START_TAG   = FName(TEXT("PlayerStart.Shadow"));

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

public:
	// ── Pawn Class Configuration ──────────────────────────────────────────────
	/** Linh — Light Weaver character class (player 1) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Characters",
		meta = (DisplayName = "Linh Character Class (Light)"))
	TSubclassOf<ASODPlayerCharacter> LinhCharacterClass;

	/** Nam — Shadow Walker character class (player 2) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Characters",
		meta = (DisplayName = "Nam Character Class (Shadow)"))
	TSubclassOf<ASODPlayerCharacter> NamCharacterClass;

	// ── Role Assignment ──────────────────────────────────────────────────────
	/**
	 * Assign role to a player character via replicated PlayerRole property.
	 * Call this from PostLogin after the pawn is possessed.
	 */
	UFUNCTION(BlueprintCallable, Category = "SOD|Role")
	void AssignPlayerRole(ASODPlayerCharacter* Character, int32 PlayerIndex);

	// ── Checkpoint / Respawn ─────────────────────────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "SOD|Checkpoint")
	void RespawnAtCheckpoint();

	UFUNCTION(BlueprintCallable, Category = "SOD|Checkpoint")
	void SetCheckpoint(FTransform NewCheckpoint);

	UFUNCTION(BlueprintPure, Category = "SOD|Session")
	int32 GetConnectedPlayerCount() const { return ConnectedPlayers; }

	// ── Puzzle State (CoopShardRegistry) ─────────────────────────────────────
	/**
	 * Called by puzzle actors (SodPuzzleActorBase) when a shard is
	 * activated or deactivated. Tracks active shards so CoopBridgeActor
	 * can poll state via GetActivatedShardCount().
	 */
	UFUNCTION(BlueprintCallable, Category = "SOD|Puzzle")
	void SetShardActivationState(FName ShardID, bool bActivated);

	UFUNCTION(BlueprintPure, Category = "SOD|Puzzle")
	TArray<FName> GetActivatedShardIDs() const { return ActivatedShardIDs; }

	UFUNCTION(BlueprintPure, Category = "SOD|Puzzle")
	int32 GetActivatedShardCount() const { return ActivatedShardIDs.Num(); }

	UFUNCTION(BlueprintCallable, Category = "SOD|Puzzle")
	void ResetAllPuzzles();

	// ── Round ────────────────────────────────────────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "SOD|Session")
	void RestartRound();

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	/** Connected player count — used for round-robin spawn */
	int32 ConnectedPlayers = 0;

	/**
	 * Currently activated shard IDs — replicated to all clients.
	 * SodPuzzleActorBase::ApplyActivationState() calls SetShardActivationState()
	 * to register shards. CoopBridgeActor::Tick() polls GetActivatedShardCount().
	 */
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ActivatedShardIDs,
		Meta = (DisplayName = "Activated Shard IDs"))
	TArray<FName> ActivatedShardIDs;

	/** Last checkpoint transform for respawn */
	UPROPERTY(Replicated)
	FTransform LastCheckpoint;

	UFUNCTION()
	void OnRep_ActivatedShardIDs();
};
