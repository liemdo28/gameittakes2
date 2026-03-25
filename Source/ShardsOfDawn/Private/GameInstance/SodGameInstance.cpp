// Copyright Shards of Dawn Team 2026

#include "GameInstance/SodGameInstance.h"
#include "EnhancedInputSubsystems.h"

USodGameInstance::USodGameInstance()
{
    LocalPlayerArchetypeIndex = 0;
}

void USodGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Log, TEXT("[SodGameInstance] Initialized — Shards of Dawn"));
}

void USodGameInstance::Shutdown()
{
    UE_LOG(LogTemp, Log, TEXT("[SodGameInstance] Shutdown"));
    Super::Shutdown();
}

void USodGameInstance::SetLocalArchetype(int32 Index)
{
    LocalPlayerArchetypeIndex = Index;
    UE_LOG(LogTemp, Log, TEXT("[SodGameInstance] Archetype set to index: %d"), Index);
}

void USodGameInstance::ApplyIMC(int32 PlayerIndex, UInputMappingContext* IMC)
{
    if (!IMC) return;

    const TArray<TObjectPtr<ULocalPlayer>>& LocalPlayers = GetLocalPlayers();
    if (LocalPlayers.IsValidIndex(PlayerIndex))
    {
        ULocalPlayer* LP = LocalPlayers[PlayerIndex];
        if (UEnhancedInputLocalPlayerSubsystem* Sub =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
        {
            Sub->AddMappingContext(IMC, 0);
        }
    }
}

void USodGameInstance::RemoveIMC(int32 PlayerIndex, UInputMappingContext* IMC)
{
    if (!IMC) return;

    const TArray<TObjectPtr<ULocalPlayer>>& LocalPlayers = GetLocalPlayers();
    if (LocalPlayers.IsValidIndex(PlayerIndex))
    {
        ULocalPlayer* LP = LocalPlayers[PlayerIndex];
        if (UEnhancedInputLocalPlayerSubsystem* Sub =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
        {
            Sub->RemoveMappingContext(IMC);
        }
    }
}

void USodGameInstance::LogGameEvent(const FString& EventName, const TMap<FString, FString>& Params)
{
    FString ParamStr;
    for (const auto& KV : Params)
    {
        ParamStr += FString::Printf(TEXT(" %s=%s"), *KV.Key, *KV.Value);
    }
    UE_LOG(LogTemp, Log, TEXT("[TELEMETRY] Event=%s%s"), *EventName, *ParamStr);
}

void USodGameInstance::AdvanceChapter()
{
    CurrentChapterIndex++;
    UE_LOG(LogTemp, Log, TEXT("[SodGameInstance] Advanced to chapter: %d"), CurrentChapterIndex);
}

void USodGameInstance::ResetProgress()
{
    CurrentChapterIndex = 0;
    CurrentSaveSlotName = TEXT("Slot_AutoSave");
    UE_LOG(LogTemp, Log, TEXT("[SodGameInstance] Progress reset."));
}
