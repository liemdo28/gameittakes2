// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SODPlayerCharacter.h"
#include "SODNamCharacter.generated.h"

/**
 * Nam - "The Shade" (Player 2, Shadow role)
 * Abilities: Shadow Anchor, Veil Step, Echo Grip
 * Gameplay identity: Anchors objects in place, sustains puzzle states, protects from light hazards.
 */
UCLASS()
class SHARDSOFDAWN_API ASODNamCharacter : public ASODPlayerCharacter
{
	GENERATED_BODY()

public:
	ASODNamCharacter();

	/** Activate Shadow Anchor: lock a target object in its current state */
	UFUNCTION(BlueprintCallable, Category = "SOD|Ability|Shadow")
	void ActivateShadowAnchor(AActor* Target);

	/** Release the currently anchored object */
	UFUNCTION(BlueprintCallable, Category = "SOD|Ability|Shadow")
	void ReleaseShadowAnchor();

	/** Is an object currently anchored? */
	UFUNCTION(BlueprintPure, Category = "SOD|Ability|Shadow")
	bool IsShadowAnchorActive() const { return AnchoredActor != nullptr; }

	/** Get the currently anchored actor */
	UFUNCTION(BlueprintPure, Category = "SOD|Ability|Shadow")
	AActor* GetAnchoredActor() const { return AnchoredActor; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Max distance to anchor a target */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Ability|Shadow")
	float ShadowAnchorRange;

	/** Max duration an anchor can be held */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SOD|Ability|Shadow")
	float ShadowAnchorMaxDuration;

private:
	UPROPERTY()
	TObjectPtr<AActor> AnchoredActor;

	float AnchorTimer;
};
