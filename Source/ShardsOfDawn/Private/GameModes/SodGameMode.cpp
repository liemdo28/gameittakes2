// Copyright Shards of Dawn Team 2026

#include "GameModes/SodGameMode.h"
#include "Actors/PuzzleActors/SodPuzzleActorBase.h"
#include "Characters/SODLinhCharacter.h"
#include "Characters/SODNamCharacter.h"
#include "Characters/SODPlayerCharacter.h"
#include "Controllers/SodPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/Interface_Interactive.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASodGameMode::ASodGameMode()
{
    PlayerControllerClass = ASodPlayerController::StaticClass();
    bReplicates = true;
    bDelayedStart = false;

    LightWeaverClass = ASODLinhCharacter::StaticClass();
    ShadowWalkerClass = ASODNamCharacter::StaticClass();
    DefaultPawnClass = ASODPlayerCharacter::StaticClass();
}

void ASodGameMode::PostLogin(APlayerController* NewPlayer)
{
    const int32 JoiningPlayerIndex = ActivePlayerCount;
    Super::PostLogin(NewPlayer);
    ActivePlayerCount++;
    AssignArchetype(JoiningPlayerIndex, Cast<ASodPlayerController>(NewPlayer));
    OnPlayerJoined(NewPlayer);
}

void ASodGameMode::Logout(AController* OldPlayer)
{
    ActivePlayerCount = FMath::Max(0, ActivePlayerCount - 1);
    OnPlayerLeft(Cast<APlayerController>(OldPlayer));
    Super::Logout(OldPlayer);
}

void ASodGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASodGameMode, ActivatedShardIDs);
}

AActor* ASodGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    if (!Player) return nullptr;

    TArray<AActor*> AllStarts;
    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), AllStarts);

    // Try to find tagged starts for archetype
    FName TargetTag = (ActivePlayerCount % 2 == 0) ? LightWeaverStartTag : ShadowWalkerStartTag;

    for (AActor* Start : AllStarts)
    {
        if (APlayerStart* PS = Cast<APlayerStart>(Start))
        {
            if (PS->PlayerStartTag == TargetTag)
            {
                return PS;
            }
        }
    }

    // Fallback: alternate between two closest unoccupied starts
    return !AllStarts.IsEmpty() ? AllStarts[ActivePlayerCount % AllStarts.Num()] : nullptr;
}

UClass* ASodGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    const bool bSpawnLightWeaver = ActivePlayerCount % 2 == 0;
    const TSubclassOf<ASODPlayerCharacter> RequestedClass = bSpawnLightWeaver ? LightWeaverClass : ShadowWalkerClass;

    return RequestedClass ? RequestedClass.Get() : Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASodGameMode::AssignArchetype(int32 PlayerIndex, ASodPlayerController* PC)
{
    if (!PC)
    {
        return;
    }

    if (APawn* Pawn = PC->GetPawn())
    {
        if (Cast<ASODPlayerCharacter>(Pawn))
        {
            PC->RefreshInputMapping();
            UE_LOG(LogTemp, Log, TEXT("[SodGameMode] Role assigned for player index %d"), PlayerIndex);
        }
    }
}

void ASodGameMode::OnPlayerJoined_Implementation(APlayerController* NewPlayer)
{
    UE_LOG(LogTemp, Log, TEXT("[SodGameMode] Player joined. Active count: %d"), ActivePlayerCount);
}

void ASodGameMode::OnPlayerLeft_Implementation(APlayerController* LeftPlayer)
{
    UE_LOG(LogTemp, Log, TEXT("[SodGameMode] Player left. Active count: %d"), ActivePlayerCount);
}

void ASodGameMode::SetShardActivationState(FName ShardID, bool bActivated)
{
    if (ShardID.IsNone())
    {
        return;
    }

    if (bActivated)
    {
        ActivatedShardIDs.AddUnique(ShardID);
    }
    else
    {
        ActivatedShardIDs.Remove(ShardID);
    }
}

void ASodGameMode::ResetAllPuzzles()
{
    ActivatedShardIDs.Empty();

    TArray<AActor*> PuzzleActors;
    UGameplayStatics::GetAllActorsOfClass(this, ASodPuzzleActorBase::StaticClass(), PuzzleActors);

    for (AActor* Actor : PuzzleActors)
    {
        if (ASodPuzzleActorBase* PuzzleActor = Cast<ASodPuzzleActorBase>(Actor))
        {
            PuzzleActor->ResetPuzzleState();
            UE_LOG(LogTemp, Log, TEXT("[SodGameMode] Reset puzzle actor: %s"), *Actor->GetName());
        }
    }
}

void ASodGameMode::RestartRound()
{
    ResetAllPuzzles();
    UE_LOG(LogTemp, Log, TEXT("[SodGameMode] Round restarted."));
}
