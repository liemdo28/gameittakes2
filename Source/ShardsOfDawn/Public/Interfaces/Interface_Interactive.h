// Copyright Shards of Dawn Team 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Interactive.generated.h"

class ASodPlayerCharacter;

/**
 * Interface for all interactive objects in the game world.
 * Implement this on any actor that should respond to player interaction.
 */
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UInterface_Interactive : public UInterface
{
    GENERATED_BODY()
};

class SHARDSOFDAWN_API IInterface_Interactive
{
    GENERATED_BODY()

public:
    /** Returns display name shown on the interaction prompt */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FText GetInteractionPrompt() const;

    /** Whether this object can currently be interacted with */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    bool CanInteract(ASodPlayerCharacter* Interactor) const;

    /** Called when player presses interact */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnInteract(ASodPlayerCharacter* Interactor);

    /** Internal execute — called from ASodPlayerCharacter::TryInteract */
    void Execute_ExecuteInteraction(AActor* Target, ASodPlayerCharacter* Interactor);

    /** Internal implementation proxy */
    static void ExecuteInteraction(AActor* Target, ASodPlayerCharacter* Interactor);
};

// ── Default implementation inline ────────────────────────────────────────────

inline void IInterface_Interactive::Execute_ExecuteInteraction(AActor* Target, ASodPlayerCharacter* Interactor)
{
    if (Target)
    {
        if (auto* Interface = Cast<IInterface_Interactive>(Target))
        {
            Interface->OnInteract(Interactor);
        }
    }
}
