// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Interactive.generated.h"

class ASODPlayerCharacter;

/**
 * IInterface_Interactive — Canonical interface for all interactive objects.
 *
 * Implemented by:
 * - ASodPuzzleActorBase (and subclasses: ALightShard, AShadowShard)
 * - ACoopBridgeActor
 * - Any other interactable world object
 *
 * C++ override convention:
 *   Declare in class .h:  void ResetPuzzleState_Implementation() override;
 *   Implement in class .cpp: calls actual logic (e.g. ApplyActivationState(false))
 * Blueprint override: implement "Reset Puzzle State" event directly in graph.
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
	/** Display text shown on the interaction prompt (e.g. "[E] Interact") */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionPrompt() const;

	/**
	 * Whether the given player can currently interact with this object.
	 * Use this to enforce archetype/role restrictions.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(ASODPlayerCharacter* Interactor) const;

	/** Called when the player presses the interact key. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(ASODPlayerCharacter* Interactor);

	/**
	 * Reset puzzle/interaction state to default.
	 * Called by ASODGameMode::ResetAllPuzzles() on round restart.
	 * Default: no-op. Override in implementing class.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction",
		meta = (DisplayName = "Reset Puzzle State"))
	void ResetPuzzleState();
};

// ── Inline default implementations ──────────────────────────────────────────────

inline FText IInterface_Interactive::GetInteractionPrompt_Implementation() const
{
	return FText::FromString(TEXT("[E] Interact"));
}

inline bool IInterface_Interactive::CanInteract_Implementation(ASODPlayerCharacter* /*Interactor*/) const
{
	return true;
}

inline void IInterface_Interactive::OnInteract_Implementation(ASODPlayerCharacter* /*Interactor*/)
{
	// Override in implementing class
}

inline void IInterface_Interactive::ResetPuzzleState_Implementation()
{
	// No-op default. Override in ASodPuzzleActorBase, ACoopBridgeActor, etc.
}
