// Copyright Shards of Dawn. All Rights Reserved.

#include "Core/SODCheatManager.h"
#include "Actors/PuzzleActors/SODPuzzleActor.h"
#include "Actors/PuzzleActors/SODPuzzleDoor.h"
#include "Combat/SODDualShieldEnemy.h"
#include "CoOp/SODCoOpSyncSubsystem.h"
#include "Telemetry/SODTelemetrySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

void USODCheatManager::SOD_SkipPuzzle()
{
	TArray<AActor*> Puzzles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASODPuzzleActor::StaticClass(), Puzzles);

	for (AActor* Actor : Puzzles)
	{
		ASODPuzzleActor* Puzzle = Cast<ASODPuzzleActor>(Actor);
		if (Puzzle && Puzzle->GetPuzzleState() != ESODPuzzleState::Solved)
		{
			Puzzle->DebugSolve();
			UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Solved puzzle: %s"), *Puzzle->GetName());
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[CHEAT] No unsolved puzzles found"));
}

void USODCheatManager::SOD_SkipAllPuzzles()
{
	TArray<AActor*> Puzzles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASODPuzzleActor::StaticClass(), Puzzles);

	int32 Solved = 0;
	for (AActor* Actor : Puzzles)
	{
		ASODPuzzleActor* Puzzle = Cast<ASODPuzzleActor>(Actor);
		if (Puzzle && Puzzle->GetPuzzleState() != ESODPuzzleState::Solved)
		{
			Puzzle->DebugSolve();
			Solved++;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Solved %d puzzles"), Solved);
}

void USODCheatManager::SOD_SkipCombat()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASODDualShieldEnemy::StaticClass(), Enemies);

	for (AActor* Actor : Enemies)
	{
		ASODDualShieldEnemy* Enemy = Cast<ASODDualShieldEnemy>(Actor);
		if (Enemy)
		{
			Enemy->DebugDefeat();
			UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Defeated enemy: %s"), *Enemy->GetName());
		}
	}
}

void USODCheatManager::SOD_GodMode()
{
	bGodModeActive = !bGodModeActive;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			ACharacter* Character = Cast<ACharacter>(PC->GetPawn());
			if (Character)
			{
				Character->SetCanBeDamaged(!bGodModeActive);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[CHEAT] God mode: %s"), bGodModeActive ? TEXT("ON") : TEXT("OFF"));
}

void USODCheatManager::SOD_ResetLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Resetting level..."));

	// Reset puzzles
	TArray<AActor*> Puzzles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASODPuzzleActor::StaticClass(), Puzzles);
	for (AActor* Actor : Puzzles)
	{
		ASODPuzzleActor* Puzzle = Cast<ASODPuzzleActor>(Actor);
		if (Puzzle)
		{
			Puzzle->ResetPuzzle();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Level reset complete"));
}

void USODCheatManager::SOD_PrintTelemetry()
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (GI)
	{
		USODTelemetrySubsystem* Telemetry = GI->GetSubsystem<USODTelemetrySubsystem>();
		if (Telemetry)
		{
			Telemetry->PrintSessionSummary();
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Telemetry subsystem not found"));
}

void USODCheatManager::SOD_SetSyncScore(float NewScore)
{
	USODCoOpSyncSubsystem* SyncSub = GetWorld()->GetSubsystem<USODCoOpSyncSubsystem>();
	if (SyncSub)
	{
		SyncSub->DebugSetSyncScore(FMath::Clamp(NewScore, 0.0f, 1.0f));
		UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Sync score set to: %.2f"), NewScore);
	}
}

void USODCheatManager::SOD_TeleportToZone(const FString& ZoneName)
{
	FVector TargetLocation = FVector::ZeroVector;

	if (ZoneName.Equals(TEXT("Start"), ESearchCase::IgnoreCase))
	{
		TargetLocation = FVector(0.0f, 0.0f, 100.0f);
	}
	else if (ZoneName.Equals(TEXT("Puzzle"), ESearchCase::IgnoreCase))
	{
		TargetLocation = FVector(2000.0f, 0.0f, 100.0f);
	}
	else if (ZoneName.Equals(TEXT("Combat"), ESearchCase::IgnoreCase))
	{
		TargetLocation = FVector(5000.0f, 0.0f, 100.0f);
	}
	else if (ZoneName.Equals(TEXT("End"), ESearchCase::IgnoreCase))
	{
		TargetLocation = FVector(8000.0f, 0.0f, 100.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Unknown zone: %s. Use: Start, Puzzle, Combat, End"), *ZoneName);
		return;
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			PC->GetPawn()->SetActorLocation(TargetLocation);
			TargetLocation += FVector(0.0f, 200.0f, 0.0f); // Offset P2
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[CHEAT] Teleported players to: %s"), *ZoneName);
}
