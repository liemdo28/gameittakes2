// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SODPrototypeLevel.generated.h"

class ASODPuzzleActor;
class ASODPuzzleDoor;
class ASODDualShieldEnemy;
class ASODCheckpointActor;
class ASODSplitScreenManager;

/**
 * Prototype level manager that assembles a testable puzzle room.
 * Spawns greybox geometry, puzzle actors, enemies, and checkpoints
 * in a layout designed for 2-player co-op smoke testing.
 *
 * TESTER FLOW:
 *   1. Both players spawn in Start Zone
 *   2. Walk to Puzzle Zone: 3 Light/Shadow puzzles + 1 door
 *   3. Solve all puzzles to open door (Light reveals, Shadow anchors)
 *   4. Enter Combat Zone: 1 Dual-Shield enemy
 *   5. Defeat enemy using co-op (break shields + finisher)
 *   6. Reach End Zone: checkpoint triggers level complete
 */
UCLASS()
class SHARDSOFDAWN_API ASODPrototypeLevel : public AActor
{
	GENERATED_BODY()

public:
	ASODPrototypeLevel();

	virtual void BeginPlay() override;

	/** Reset the entire level (all puzzles, enemies, checkpoints) */
	UFUNCTION(BlueprintCallable, Category = "SOD|Level")
	void ResetLevel();

	/** Get elapsed time since level start */
	UFUNCTION(BlueprintPure, Category = "SOD|Level")
	float GetElapsedTime() const;

	/** Get number of puzzles solved */
	UFUNCTION(BlueprintPure, Category = "SOD|Level")
	int32 GetPuzzlesSolved() const { return PuzzlesSolved; }

	/** Has the combat encounter been completed? */
	UFUNCTION(BlueprintPure, Category = "SOD|Level")
	bool IsCombatComplete() const { return bCombatComplete; }

	/** Is the level finished (all objectives done)? */
	UFUNCTION(BlueprintPure, Category = "SOD|Level")
	bool IsLevelComplete() const { return bLevelComplete; }

protected:
	UFUNCTION()
	void OnPuzzleSolved(ESODPuzzleState NewState);

	UFUNCTION()
	void OnEnemyDefeated(ESODShieldPhase NewPhase);

	UFUNCTION()
	void OnFinalCheckpointReached();

	/** Puzzle actors in the level */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SOD|Level")
	TArray<TObjectPtr<ASODPuzzleActor>> Puzzles;

	/** The door that opens when puzzles are solved */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SOD|Level")
	TObjectPtr<ASODPuzzleDoor> PuzzleDoor;

	/** Combat encounter enemy */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SOD|Level")
	TObjectPtr<ASODDualShieldEnemy> CombatEnemy;

	/** Final checkpoint */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SOD|Level")
	TObjectPtr<ASODCheckpointActor> FinalCheckpoint;

	/** Split-screen manager reference */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SOD|Level")
	TObjectPtr<ASODSplitScreenManager> SplitScreenManager;

private:
	int32 PuzzlesSolved;
	bool bCombatComplete;
	bool bLevelComplete;
	float LevelStartTime;
};
