// Copyright Shards of Dawn. All Rights Reserved.

#include "Core/SODGameMode.h"
#include "Characters/SODPlayerCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ASODGameMode::ASODGameMode()
	: ConnectedPlayers(0)
{
	// Default pawn will be overridden per-player based on role
	DefaultPawnClass = nullptr;
}

void ASODGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ConnectedPlayers++;

	// First player = Linh (Light), second player = Nam (Shadow)
	TSubclassOf<ASODPlayerCharacter> CharClass = (ConnectedPlayers == 1) ? LinhCharacterClass : NamCharacterClass;

	if (CharClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* StartSpot = ChoosePlayerStart(NewPlayer);
		FTransform SpawnTransform = StartSpot ? StartSpot->GetActorTransform() : FTransform::Identity;

		ASODPlayerCharacter* NewChar = GetWorld()->SpawnActor<ASODPlayerCharacter>(CharClass, SpawnTransform, SpawnParams);
		if (NewChar)
		{
			NewPlayer->Possess(NewChar);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("SOD: Player %d connected. Role: %s"), ConnectedPlayers, ConnectedPlayers == 1 ? TEXT("Linh (Light)") : TEXT("Nam (Shadow)"));
}

void ASODGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ConnectedPlayers = FMath::Max(0, ConnectedPlayers - 1);
}

void ASODGameMode::RespawnAtCheckpoint()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			PC->GetPawn()->SetActorTransform(LastCheckpoint);
		}
	}
}

void ASODGameMode::SetCheckpoint(FTransform NewCheckpoint)
{
	LastCheckpoint = NewCheckpoint;
}

AActor* ASODGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Starts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);

	// Use different starts for each player
	int32 PlayerIndex = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (It->Get() == Player)
		{
			break;
		}
		PlayerIndex++;
	}

	if (Starts.IsValidIndex(PlayerIndex))
	{
		return Starts[PlayerIndex];
	}
	return Starts.Num() > 0 ? Starts[0] : nullptr;
}
