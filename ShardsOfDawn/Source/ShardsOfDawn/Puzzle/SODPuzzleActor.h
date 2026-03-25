// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SODPuzzleActor.generated.h"

class ASODPlayerCharacter;

/** The dual-state of a puzzle element (core mechanic) */
UENUM(BlueprintType)
enum class ESODPuzzleState : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	LightRevealed UMETA(DisplayName = "Light Revealed"),
	ShadowAnchored UMETA(DisplayName = "Shadow Anchored"),
	Solved UMETA(DisplayName = "Solved")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleStateChanged, ESODPuzzleState, NewState);

/**
 * Base puzzle actor for dual-state Light/Shadow puzzles.
 * Flow: Neutral -> Linh uses Light Pulse -> LightRevealed -> Nam uses Shadow Anchor -> Solved
 * Both players must cooperate; neither can solve alone.
 */
UCLASS()
class SHARDSOFDAWN_API ASODPuzzleActor : public AActor
{
	GENERATED_BODY()

public:
	ASODPuzzleActor();

	/** Called when Light ability hits this actor */
	UFUNCTION(BlueprintCallable, Category = "SOD|Puzzle")
	void OnLightInteract(ASODPlayerCharacter* LightPlayer);

	/** Called when Shadow ability hits this actor */
	UFUNCTION(BlueprintCallable, Category = "SOD|Puzzle")
	void OnShadowInteract(ASODPlayerCharacter* ShadowPlayer);

	/** Reset puzzle to neutral state */
	UFUNCTION(BlueprintCallable, Category = "SOD|Puzzle")
	void ResetPuzzle();

	/** Get current puzzle state */
	UFUNCTION(BlueprintPure, Category = "SOD|Puzzle")
	ESODPuzzleState GetPuzzleState() const { return CurrentState; }

	/** Event fired when state changes */
	UPROPERTY(BlueprintAssignable, Category = "SOD|Puzzle")
	FOnPuzzleStateChanged OnPuzzleStateChanged;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentState();

	/** Set new state and broadcast */
	void SetPuzzleState(ESODPuzzleState NewState);

	/** Called when puzzle is solved - override for custom behavior */
	UFUNCTION(BlueprintNativeEvent, Category = "SOD|Puzzle")
	void OnPuzzleSolved();

	/** Called when light reveals this puzzle */
	UFUNCTION(BlueprintNativeEvent, Category = "SOD|Puzzle")
	void OnLightRevealed();

	/** Called when shadow anchors this puzzle */
	UFUNCTION(BlueprintNativeEvent, Category = "SOD|Puzzle")
	void OnShadowLocked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SOD|Puzzle")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SOD|Puzzle")
	TObjectPtr<UStaticMeshComponent> PuzzleMesh;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState, BlueprintReadOnly, Category = "SOD|Puzzle")
	ESODPuzzleState CurrentState;

	/** Time window (seconds) after Light reveal before it resets to Neutral */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SOD|Puzzle")
	float RevealTimeWindow;

	/** If true, puzzle can be re-solved after reset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SOD|Puzzle")
	bool bResettable;

private:
	FTimerHandle RevealTimerHandle;
};
