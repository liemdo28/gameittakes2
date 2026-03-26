// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "SODCheatManager.generated.h"

/**
 * Debug cheat manager for testers.
 * Console commands (press ~ to open console):
 *
 *   SOD.SkipPuzzle       - Solve the nearest puzzle instantly
 *   SOD.SkipAllPuzzles   - Solve all puzzles and open all doors
 *   SOD.SkipCombat       - Defeat current enemy instantly
 *   SOD.GodMode          - Toggle invincibility for both players
 *   SOD.ResetLevel       - Reset the entire prototype level
 *   SOD.PrintTelemetry   - Print current session telemetry
 *   SOD.SetSyncScore X   - Override co-op sync score (0.0 - 1.0)
 *   SOD.TeleportToZone X - Teleport both players (Start/Puzzle/Combat/End)
 */
UCLASS()
class SHARDSOFDAWN_API USODCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void SOD_SkipPuzzle();

	UFUNCTION(Exec)
	void SOD_SkipAllPuzzles();

	UFUNCTION(Exec)
	void SOD_SkipCombat();

	UFUNCTION(Exec)
	void SOD_GodMode();

	UFUNCTION(Exec)
	void SOD_ResetLevel();

	UFUNCTION(Exec)
	void SOD_PrintTelemetry();

	UFUNCTION(Exec)
	void SOD_SetSyncScore(float NewScore);

	UFUNCTION(Exec)
	void SOD_TeleportToZone(const FString& ZoneName);

private:
	bool bGodModeActive = false;
};
