// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SodPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * SodPlayerController — Handles input mapping context switching per archetype
 * and manages player HUD / UI state.
 */
UCLASS()
class SHARDSOFDAWN_API ASodPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ASodPlayerController();

    // ── Input Mapping Contexts ──────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TSoftObjectPtr<UInputMappingContext> LightWeaver_IMC;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TSoftObjectPtr<UInputMappingContext> ShadowWalker_IMC;

    // ── HUD ──────────────────────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class USodHUD> HUDClass;

    // ── UI State ─────────────────────────────────────────────────
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowInteractionPrompt(const FText& PromptText);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideInteractionPrompt();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowArchetypeNotification(const FText& Title, const FText& Body, float Duration = 3.0f);

    // ── Archetype Sync ───────────────────────────────────────────
    UFUNCTION(Server, Reliable, Category = "Network")
    void Server_NotifyArchetypeSet(uint8 ArchetypeValue);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void RefreshInputMapping();

protected:
    void BeginPlay() override;
    void SetupInputComponent() override;
    void OnPossess(APawn* InPawn) override;

private:
    TObjectPtr<class USodGameInstance> GameInstance;
};
