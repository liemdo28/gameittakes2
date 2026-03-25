// Copyright Shards of Dawn Team 2026

#include "GameModes/SodGameMode.h"
#include "Characters/SodPlayerCharacter.h"
#include "Controllers/SodPlayerController.h"
#include "Kismet/GameplayStatics.h"

ASodGameMode::ASodGameMode()
{
    PlayerControllerClass = ASodPlayerController::StaticClass();
    bReplicates = true;
    bDelayedStart = false;

    // Default pawn class will be set via Blueprint or config
    DefaultPawnClass = ASodPlayerCharacter::StaticClass();
}

void ASodGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    ActivePlayerCount++;
    OnPlayerJoined(NewPlayer);
}

void ASodGameMode::Logout(AController* OldPlayer)
{
    ActivePlayerCount--;
    OnPlayerLeft(Cast<APlayerController>(OldPlayer));
    Super::Logout(OldPlayer);
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

void ASodGameMode::AssignArchetype(int32 PlayerIndex, ASodPlayerController* PC)
{
    if (!PC) return;

    EPlayerArchetype Archetype = (PlayerIndex % 2 == 0)
        ? EPlayerArchetype::LightWeaver
        : EPlayerArchetype::ShadowWalker;

    if (APawn* Pawn = PC->GetPawn())
    {
        if (ASodPlayerCharacter* Char = Cast<ASodPlayerCharacter>(Pawn))
        {
            Char->Archetype = Archetype;
            Char->RepArchetype = Archetype;
            Char->OnRep_ArchetypeChanged();
        }
    }
}

void ASodGameMode::Generic_PawnSpawner(APlayerController* NewPlayerController)
{
    if (!NewPlayerController) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.Instigator = GetInstigator();

    UClass* PawnClass = (ActivePlayerCount % 2 == 0)
        ? LightWeaverClass.Get()
        : ShadowWalkerClass.Get();

    if (!PawnClass) PawnClass = DefaultPawnClass;

    FVector SpawnLoc = FVector(0.0f, 0.0f, 200.0f);
    FRotator SpawnRot = FRotator(0.0f, 0.0f, 0.0f);

    if (APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnLoc, SpawnRot, SpawnParams))
    {
        NewPlayerController->Possess(NewPawn);
        AssignArchetype(ActivePlayerCount, Cast<ASodPlayerController>(NewPlayerController));
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

void ASodGameMode::RegisterActivatedShard(FName ShardID)
{
    if (!ActivatedShardIDs.Contains(ShardID))
    {
        ActivatedShardIDs.Add(ShardID);
    }
}

void ASodGameMode::ResetAllPuzzles()
{
    ActivatedShardIDs.Empty();

    TArray<AActor*> PuzzleActors;
    UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), PuzzleActors);

    for (AActor* Actor : PuzzleActors)
    {
        if (Actor->GetClass()->ImplementsInterface(UInterface_Interactive::StaticClass()))
        {
            // Reset via interaction if the actor supports it
            UE_LOG(LogTemp, Log, TEXT("[SodGameMode] Reset puzzle actor: %s"), *Actor->GetName());
        }
    }
}

void ASodGameMode::RestartRound()
{
    ResetAllPuzzles();
    UE_LOG(LogTemp, Log, TEXT("[SodGameMode] Round restarted."));
}
