// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SodGameMode.generated.h"

class ASodPlayerController;
class ASodPlayerCharacter;

/**
 * SodGameMode — Entry game mode for Shards of Dawn prototype.
 * Handles player spawning, archetype assignment, and co-op state management.
 */
UCLASS()
class SHARDSOFDAWN_API ASodGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    ASodGameMode();

    // ── Player Spawning ──────────────────────────────────────────
    AActor* ChoosePlayerStart_Implementation(AController* Player) override;
    UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    // ── Archetype Assignment ─────────────────────────────────────
    /** Assigns archetype to player based on index (0 = Light Weaver, 1 = Shadow Walker) */
    UFUNCTION(BlueprintCallable, Category = "Archetype")
    void AssignArchetype(int32 PlayerIndex, class ASodPlayerController* PC);

    // ── Co-op State ─────────────────────────────────────────────
    UPROPERTY(BlueprintReadOnly, Category = "Coop")
    int32 ActivePlayerCount = 0;

    UFUNCTION(BlueprintNativeEvent, Category = "Coop")
    void OnPlayerJoined(APlayerController* NewPlayer);

    UFUNCTION(BlueprintNativeEvent, Category = "Coop")
    void OnPlayerLeft(APlayerController* LeftPlayer);

    // ── Puzzle State ─────────────────────────────────────────────
    /** Tracks which shards are currently activated */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Puzzle")
    TArray<FName> ActivatedShardIDs;

    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void SetShardActivationState(FName ShardID, bool bActivated);

    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void ResetAllPuzzles();

    // ── Round / Session ─────────────────────────────────────────
    UFUNCTION(BlueprintCallable, Category = "Session")
    void RestartRound();

protected:
    void PostLogin(APlayerController* NewPlayer) override;
    void Logout(AController* OldPlayer) override;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ── Config ───────────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<ASodPlayerCharacter> LightWeaverClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<ASodPlayerCharacter> ShadowWalkerClass;

    /** Player start tags used for archetype assignment */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    FName LightWeaverStartTag = FName(TEXT("PlayerStart.Light"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    FName ShadowWalkerStartTag = FName(TEXT("PlayerStart.Shadow"));
};
