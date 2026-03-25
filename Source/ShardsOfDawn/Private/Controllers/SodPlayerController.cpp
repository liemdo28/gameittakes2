// Copyright Shards of Dawn Team 2026

#include "Controllers/SodPlayerController.h"
#include "Characters/SodPlayerCharacter.h"
#include "GameInstance/SodGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

ASodPlayerController::ASodPlayerController()
{
    bReplicates = true;
}

void ASodPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Apply archetype-specific IMC if we already have a pawn
    if (APawn* Pawn = GetPawn())
    {
        if (ASodPlayerCharacter* Char = Cast<ASodPlayerCharacter>(Pawn))
        {
            RefreshInputMapping();
        }
    }
}

void ASodPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // IMC binding handled via Player subsystem in SodGameInstance
}

void ASodPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    RefreshInputMapping();
}

void ASodPlayerController::RefreshInputMapping()
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->ClearAllMappings();

        if (ASodPlayerCharacter* Char = Cast<ASodPlayerCharacter>(GetPawn()))
        {
            if (Char->Archetype == EPlayerArchetype::LightWeaver && !LightWeaver_IMC.IsNull())
            {
                Subsystem->AddMappingContext(LightWeaver_IMC.LoadSynchronous(), 0);
            }
            else if (Char->Archetype == EPlayerArchetype::ShadowWalker && !ShadowWalker_IMC.IsNull())
            {
                Subsystem->AddMappingContext(ShadowWalker_IMC.LoadSynchronous(), 0);
            }
        }
    }
}

void ASodPlayerController::ShowInteractionPrompt(const FText& PromptText)
{
    // Implemented via Blueprint or UMG in production; placeholder for C++ smoke build
    UE_LOG(LogTemp, Log, TEXT("[SodPlayerController] Interaction Prompt: %s"), *PromptText.ToString());
}

void ASodPlayerController::HideInteractionPrompt()
{
    UE_LOG(LogTemp, Log, TEXT("[SodPlayerController] Hide Interaction Prompt"));
}

void ASodPlayerController::ShowArchetypeNotification(const FText& Title, const FText& Body, float Duration)
{
    UE_LOG(LogTemp, Log, TEXT("[SodPlayerController] Archetype Notification — %s: %s"),
        *Title.ToString(), *Body.ToString());
}

void ASodPlayerController::Server_NotifyArchetypeSet_Implementation(uint8 ArchetypeValue)
{
    UE_LOG(LogTemp, Log, TEXT("[SodPlayerController] Archetype set: %d"), ArchetypeValue);
}
