// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Puzzle/SODPuzzleActor.h"
#include "SODPuzzleDoor.generated.h"

/**
 * A door that opens when its linked puzzle(s) are solved.
 * Supports requiring multiple puzzles to open (e.g., solve 3 light/shadow puzzles to proceed).
 */
UCLASS()
class SHARDSOFDAWN_API ASODPuzzleDoor : public AActor
{
	GENERATED_BODY()

public:
	ASODPuzzleDoor();

	/** Check if all required puzzles are solved */
	UFUNCTION(BlueprintPure, Category = "SOD|Puzzle")
	bool AreAllPuzzlesSolved() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnLinkedPuzzleStateChanged(ESODPuzzleState NewState);

	/** Open the door (play animation, move mesh, etc.) */
	UFUNCTION(BlueprintNativeEvent, Category = "SOD|Puzzle")
	void OpenDoor();

	/** Close the door */
	UFUNCTION(BlueprintNativeEvent, Category = "SOD|Puzzle")
	void CloseDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	/** Puzzles that must all be solved to open this door */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SOD|Puzzle")
	TArray<TObjectPtr<ASODPuzzleActor>> LinkedPuzzles;

	/** Target location when door is open (relative offset) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SOD|Puzzle")
	FVector OpenOffset;

	/** How fast the door opens */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SOD|Puzzle")
	float OpenSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|Puzzle")
	bool bIsOpen;
};
