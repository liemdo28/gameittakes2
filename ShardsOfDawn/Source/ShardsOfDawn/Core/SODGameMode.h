// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SODGameMode.generated.h"

class ASODPlayerCharacter;

/**
 * Core game mode for Shards of Dawn co-op gameplay.
 * Manages player spawning, role assignment (Light/Shadow), and checkpoint system.
 */
UCLASS()
class SHARDSOFDAWN_API ASODGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASODGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	/** Respawn both players at the last checkpoint */
	UFUNCTION(BlueprintCallable, Category = "SOD|Checkpoint")
	void RespawnAtCheckpoint();

	/** Set active checkpoint transform */
	UFUNCTION(BlueprintCallable, Category = "SOD|Checkpoint")
	void SetCheckpoint(FTransform NewCheckpoint);

	/** Get number of connected players */
	UFUNCTION(BlueprintPure, Category = "SOD|Session")
	int32 GetConnectedPlayerCount() const { return ConnectedPlayers; }

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Characters")
	TSubclassOf<ASODPlayerCharacter> LinhCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Characters")
	TSubclassOf<ASODPlayerCharacter> NamCharacterClass;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|Checkpoint")
	FTransform LastCheckpoint;

	int32 ConnectedPlayers;
};
