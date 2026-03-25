// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SodGameInstance.generated.h"

class UInputMappingContext;

/**
 * SodGameInstance — Session-level manager for archetype state,
 * IMC switching, and cross-level persistence.
 */
UCLASS()
class SHARDSOFDAWN_API USodGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    USodGameInstance();

    // ── Session / Archetype ─────────────────────────────────────
    UPROPERTY(BlueprintReadWrite, Category = "Session")
    int32 LocalPlayerArchetypeIndex = 0;

    UFUNCTION(BlueprintCallable, Category = "Session")
    void SetLocalArchetype(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Session")
    int32 GetLocalArchetype() const { return LocalPlayerArchetypeIndex; }

    // ── Input Mapping ───────────────────────────────────────────
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ApplyIMC(int32 PlayerIndex, UInputMappingContext* IMC);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void RemoveIMC(int32 PlayerIndex, UInputMappingContext* IMC);

    // ── Logging / Telemetry ─────────────────────────────────────
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void LogGameEvent(const FString& EventName, const TMap<FString, FString>& Params);

    // ── Chapter / Progress ──────────────────────────────────────
    UPROPERTY(BlueprintReadWrite, Category = "Progress")
    int32 CurrentChapterIndex = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Progress")
    FString CurrentSaveSlotName = TEXT("Slot_AutoSave");

    UFUNCTION(BlueprintCallable, Category = "Progress")
    void AdvanceChapter();

    UFUNCTION(BlueprintCallable, Category = "Progress")
    void ResetProgress();

protected:
    virtual void Init() override;
    virtual void Shutdown() override;
};
