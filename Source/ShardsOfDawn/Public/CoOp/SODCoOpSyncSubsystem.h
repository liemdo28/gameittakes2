// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SODCoOpSyncSubsystem.generated.h"

/**
 * Subsystem tracking co-op synchronization metrics.
 * Drives adaptive music, VO variants, and difficulty assist.
 *
 * Co-op Sync Score (0.0 - 1.0):
 *   - Increases when players cooperate well (solve puzzles fast, coordinate abilities)
 *   - Decreases on failures, missed timing, player down
 *   - Affects: music layers, dialogue tone, subtle assist level
 */
UCLASS()
class SHARDSOFDAWN_API USODCoOpSyncSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Record a successful co-op action (puzzle solve, finisher, ability combo) */
	UFUNCTION(BlueprintCallable, Category = "SOD|CoOp")
	void RecordCoOpSuccess(float Weight = 1.0f);

	/** Record a co-op failure (missed timing, player down, puzzle timeout) */
	UFUNCTION(BlueprintCallable, Category = "SOD|CoOp")
	void RecordCoOpFailure(float Weight = 1.0f);

	/** Get the current sync score */
	UFUNCTION(BlueprintPure, Category = "SOD|CoOp")
	float GetSyncScore() const { return SyncScore; }

	/** Get the adaptive difficulty multiplier (higher sync = harder, lower = easier) */
	UFUNCTION(BlueprintPure, Category = "SOD|CoOp")
	float GetDifficultyMultiplier() const;

	/** Get the music intensity layer (0 = ambient, 1 = action, 2 = climax) */
	UFUNCTION(BlueprintPure, Category = "SOD|CoOp")
	int32 GetMusicIntensityLayer() const;

	/** Total successful co-op actions this session */
	UFUNCTION(BlueprintPure, Category = "SOD|CoOp")
	int32 GetTotalSuccesses() const { return TotalSuccesses; }

	/** Total failures this session */
	UFUNCTION(BlueprintPure, Category = "SOD|CoOp")
	int32 GetTotalFailures() const { return TotalFailures; }

	/** Debug: override sync score directly */
	void DebugSetSyncScore(float NewScore) { SyncScore = NewScore; }

private:
	float SyncScore;
	int32 TotalSuccesses;
	int32 TotalFailures;

	static constexpr float SuccessIncrement = 0.05f;
	static constexpr float FailureDecrement = 0.08f;
	static constexpr float ScoreDecayRate = 0.001f;
};
