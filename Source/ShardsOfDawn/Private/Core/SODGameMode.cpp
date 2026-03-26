// Copyright Shards of Dawn. All Rights Reserved.

#include "Core/SODGameMode.h"
#include "Characters/SODPlayerCharacter.h"
#include "Controllers/SodPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/Interface_Interactive.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

// ── Lifecycle ─────────────────────────────────────────────────────────────────

ASODGameMode::ASODGameMode()
	: ConnectedPlayers(0)
{
	DefaultPawnClass = nullptr;
	bReplicates = true;
	bDelayedStart = false;
	PlayerControllerClass = ASodPlayerController::StaticClass();
}

// ── Player Session ────────────────────────────────────────────────────────────

void ASODGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const int32 ThisPlayerIndex = ConnectedPlayers;
	ConnectedPlayers++;

	UE_LOG(LogTemp, Log,
		TEXT("[SODGameMode] Player %d connected. Assigning role: %s"),
		ThisPlayerIndex + 1,
		(ThisPlayerIndex % 2 == 0) ? TEXT("Linh (Light)") : TEXT("Nam (Shadow)"));

	// Determine character class based on player index
	TSubclassOf<ASODPlayerCharacter> CharClass =
		(ThisPlayerIndex % 2 == 0) ? LinhCharacterClass : NamCharacterClass;

	if (!CharClass)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[SODGameMode] Character class not set for player %d — using default"),
			ThisPlayerIndex + 1);
		CharClass = ASODPlayerCharacter::StaticClass();
	}

	AActor* StartSpot = ChoosePlayerStart(NewPlayer);
	FTransform SpawnTransform = StartSpot
		? StartSpot->GetActorTransform()
		: FTransform::Identity;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (ASODPlayerCharacter* NewChar = GetWorld()->SpawnActor<ASODPlayerCharacter>(
		CharClass, SpawnTransform, SpawnParams))
	{
		NewPlayer->Possess(NewChar);
		AssignPlayerRole(NewChar, ThisPlayerIndex);
	}
}

void ASODGameMode::Logout(AController* Exiting)
{
	ConnectedPlayers = FMath::Max(0, ConnectedPlayers - 1);
	Super::Logout(Exiting);
}

// ── Role Assignment ───────────────────────────────────────────────────────────

void ASODGameMode::AssignPlayerRole(ASODPlayerCharacter* Character, int32 PlayerIndex)
{
	if (!Character) return;

	const bool bIsLight = (PlayerIndex % 2 == 0);
	Character->SetPlayerRole(bIsLight ? ESODPlayerRole::Light : ESODPlayerRole::Shadow);

	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
	{
		if (ASodPlayerController* SODPC = Cast<ASodPlayerController>(PC))
		{
			SODPC->RefreshInputMapping();
		}
	}

	UE_LOG(LogTemp, Log,
		TEXT("[SODGameMode] Assigned role %s to character (PlayerIndex=%d)"),
		bIsLight ? TEXT("Light") : TEXT("Shadow"), PlayerIndex);
}

// ── PlayerStart Selection ─────────────────────────────────────────────────────

AActor* ASODGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (!Player) return nullptr;

	TArray<AActor*> AllStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AllStarts);
	if (AllStarts.IsEmpty()) return nullptr;

	int32 AlreadySeated = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (It->Get() == Player) break;
		AlreadySeated++;
	}

	const bool bIsLightPlayer = (AlreadySeated % 2 == 0);
	const FName TargetTag = bIsLightPlayer ? LINH_START_TAG : NAM_START_TAG;

	for (AActor* Start : AllStarts)
	{
		if (APlayerStart* PS = Cast<APlayerStart>(Start))
		{
			if (PS->PlayerStartTag == TargetTag)
				return PS;
		}
	}

	// Fallback: round-robin
	return AllStarts[AlreadySeated % AllStarts.Num()];
}

UClass* ASODGameMode::GetDefaultPawnClassForController_Implementation(AController* /*InController*/)
{
	// Custom spawning is handled in PostLogin — prevent AGameMode auto-spawn
	return nullptr;
}

// ── Checkpoint / Respawn ────────────────────────────────────────────────────

void ASODGameMode::RespawnAtCheckpoint()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				Pawn->SetActorTransform(LastCheckpoint, false, nullptr, ETeleportType::TeleportPhysics);
			}
		}
	}
}

void ASODGameMode::SetCheckpoint(FTransform NewCheckpoint)
{
	LastCheckpoint = NewCheckpoint;
	UE_LOG(LogTemp, Log, TEXT("[SODGameMode] Checkpoint set at %s"), *NewCheckpoint.GetLocation().ToString());
}

// ── Puzzle State (CoopShardRegistry) ─────────────────────────────────────────

void ASODGameMode::SetShardActivationState(FName ShardID, bool bActivated)
{
	if (ShardID.IsNone()) return;

	if (bActivated)
	{
		ActivatedShardIDs.AddUnique(ShardID);
	}
	else
	{
		ActivatedShardIDs.Remove(ShardID);
	}

	// OnRep_ActivatedShardIDs fires automatically via DOREPLIFETIME
}

void ASODGameMode::OnRep_ActivatedShardIDs()
{
	UE_LOG(LogTemp, VeryVerbose,
		TEXT("[SODGameMode] Replicated shard update: %d active"),
		ActivatedShardIDs.Num());
}

void ASODGameMode::ResetAllPuzzles()
{
	ActivatedShardIDs.Empty();

	TArray<AActor*> PuzzleActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), PuzzleActors);

	for (AActor* Actor : PuzzleActors)
	{
		if (Actor && Actor->Implements<UInterface_Interactive>())
		{
			IInterface_Interactive::Execute_ResetPuzzleState(Actor);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[SODGameMode] All puzzles reset."));
}

void ASODGameMode::RestartRound()
{
	ResetAllPuzzles();
	UE_LOG(LogTemp, Log, TEXT("[SODGameMode] Round restarted."));
}

// ── Replication ───────────────────────────────────────────────────────────────

void ASODGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASODGameMode, ActivatedShardIDs);
	DOREPLIFETIME(ASODGameMode, LastCheckpoint);
}
