// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SODCheckpointActor.generated.h"

/**
 * Checkpoint actor for the co-op checkpoint system.
 * Activates when both players enter the trigger volume.
 * Supports micro (puzzle cluster) and encounter (boss phase) checkpoints.
 */
UCLASS()
class SHARDSOFDAWN_API ASODCheckpointActor : public AActor
{
	GENERATED_BODY()

public:
	ASODCheckpointActor();

	UFUNCTION(BlueprintPure, Category = "SOD|Checkpoint")
	bool IsActivated() const { return bActivated; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> TriggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SOD|Checkpoint")
	bool bRequireBothPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|Checkpoint")
	bool bActivated;

private:
	int32 PlayersInVolume;
	void ActivateCheckpoint();
};
