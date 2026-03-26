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
 * Blueprint convention: implement via BlueprintNativeEvent methods.
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
	 * Called by ASODGameMode::ResetAllPuzzles().
	 * Default no-op — override in Blueprint or C++ if needed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void ResetPuzzleState();
};
