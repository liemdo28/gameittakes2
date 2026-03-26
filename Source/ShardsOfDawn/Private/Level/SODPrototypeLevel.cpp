// Copyright Shards of Dawn. All Rights Reserved.

#include "Level/SODPrototypeLevel.h"
#include "Actors/PuzzleActors/SODPuzzleActor.h"
#include "Actors/PuzzleActors/SODPuzzleDoor.h"
#include "Combat/SODDualShieldEnemy.h"
#include "CoOp/SODCheckpointActor.h"
#include "Camera/SODSplitScreenManager.h"
#include "CoOp/SODCoOpSyncSubsystem.h"
#include "Kismet/GameplayStatics.h"

ASODPrototypeLevel::ASODPrototypeLevel()
	: PuzzlesSolved(0)
	, bCombatComplete(false)
	, bLevelComplete(false)
	, LevelStartTime(0.0f)
{
}

void ASODPrototypeLevel::BeginPlay()
{
	Super::BeginPlay();

	LevelStartTime = GetWorld()->GetTimeSeconds();

	// Bind puzzle events
	for (ASODPuzzleActor* Puzzle : Puzzles)
	{
		if (Puzzle)
		{
			Puzzle->OnPuzzleStateChanged.AddDynamic(this, &ASODPrototypeLevel::OnPuzzleSolved);
		}
	}

	// Bind combat event
	if (CombatEnemy)
	{
		CombatEnemy->OnShieldPhaseChanged.AddDynamic(this, &ASODPrototypeLevel::OnEnemyDefeated);
	}

	UE_LOG(LogTemp, Log, TEXT("SOD: Prototype level started. Puzzles: %d, Combat: %s"),
		Puzzles.Num(), CombatEnemy ? TEXT("Yes") : TEXT("No"));
}

void ASODPrototypeLevel::OnPuzzleSolved(ESODPuzzleState NewState)
{
	if (NewState == ESODPuzzleState::Solved)
	{
		PuzzlesSolved++;

		// Record co-op success
		USODCoOpSyncSubsystem* SyncSub = GetWorld()->GetSubsystem<USODCoOpSyncSubsystem>();
		if (SyncSub)
		{
			SyncSub->RecordCoOpSuccess(1.0f);
		}

		UE_LOG(LogTemp, Log, TEXT("SOD: Puzzle solved! (%d/%d)"), PuzzlesSolved, Puzzles.Num());

		// Check if all puzzles done
		if (PuzzleDoor && PuzzleDoor->AreAllPuzzlesSolved())
		{
			UE_LOG(LogTemp, Log, TEXT("SOD: All puzzles solved! Door opening."));
		}
	}
}

void ASODPrototypeLevel::OnEnemyDefeated(ESODShieldPhase NewPhase)
{
	if (NewPhase == ESODShieldPhase::Defeated)
	{
		bCombatComplete = true;

		USODCoOpSyncSubsystem* SyncSub = GetWorld()->GetSubsystem<USODCoOpSyncSubsystem>();
		if (SyncSub)
		{
			SyncSub->RecordCoOpSuccess(2.0f); // Boss kill = higher weight
		}

		UE_LOG(LogTemp, Log, TEXT("SOD: Combat encounter defeated! Time: %.1fs"), GetElapsedTime());
	}
}

void ASODPrototypeLevel::OnFinalCheckpointReached()
{
	if (!bLevelComplete)
	{
		bLevelComplete = true;

		float CompletionTime = GetElapsedTime();
		USODCoOpSyncSubsystem* SyncSub = GetWorld()->GetSubsystem<USODCoOpSyncSubsystem>();
		float FinalSync = SyncSub ? SyncSub->GetSyncScore() : 0.0f;

		UE_LOG(LogTemp, Warning, TEXT("=== SOD PROTOTYPE LEVEL COMPLETE ==="));
		UE_LOG(LogTemp, Warning, TEXT("  Time: %.1f seconds"), CompletionTime);
		UE_LOG(LogTemp, Warning, TEXT("  Puzzles: %d/%d"), PuzzlesSolved, Puzzles.Num());
		UE_LOG(LogTemp, Warning, TEXT("  Combat: %s"), bCombatComplete ? TEXT("Defeated") : TEXT("Skipped"));
		UE_LOG(LogTemp, Warning, TEXT("  Co-op Sync: %.0f%%"), FinalSync * 100.0f);
		UE_LOG(LogTemp, Warning, TEXT("  Successes: %d | Failures: %d"),
			SyncSub ? SyncSub->GetTotalSuccesses() : 0,
			SyncSub ? SyncSub->GetTotalFailures() : 0);
		UE_LOG(LogTemp, Warning, TEXT("===================================="));
	}
}

void ASODPrototypeLevel::ResetLevel()
{
	PuzzlesSolved = 0;
	bCombatComplete = false;
	bLevelComplete = false;
	LevelStartTime = GetWorld()->GetTimeSeconds();

	for (ASODPuzzleActor* Puzzle : Puzzles)
	{
		if (Puzzle)
		{
			Puzzle->ResetPuzzle();
		}
	}

	UE_LOG(LogTemp, Log, TEXT("SOD: Prototype level reset."));
}

float ASODPrototypeLevel::GetElapsedTime() const
{
	return GetWorld()->GetTimeSeconds() - LevelStartTime;
}
