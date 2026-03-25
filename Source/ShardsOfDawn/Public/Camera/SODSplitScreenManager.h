// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SODSplitScreenManager.generated.h"

class ASODPlayerCharacter;

/** Split-screen display mode */
UENUM(BlueprintType)
enum class ESODSplitScreenMode : uint8
{
	Merged UMETA(DisplayName = "Merged - Shared Camera"),
	SplitHorizontal UMETA(DisplayName = "Split Horizontal"),
	SplitVertical UMETA(DisplayName = "Split Vertical"),
	Dynamic UMETA(DisplayName = "Dynamic - Auto Switch")
};

/**
 * Dynamic split-screen camera manager.
 * When players are close: merged single camera.
 * When players separate: dynamic split with geometric divider.
 * Supports hero shots per chapter.
 */
UCLASS()
class SHARDSOFDAWN_API ASODSplitScreenManager : public AActor
{
	GENERATED_BODY()

public:
	ASODSplitScreenManager();

	virtual void Tick(float DeltaTime) override;

	/** Force a specific split-screen mode */
	UFUNCTION(BlueprintCallable, Category = "SOD|Camera")
	void SetSplitMode(ESODSplitScreenMode Mode);

	/** Get current split mode */
	UFUNCTION(BlueprintPure, Category = "SOD|Camera")
	ESODSplitScreenMode GetCurrentMode() const { return CurrentMode; }

	/** Trigger a hero shot (cinematic camera angle for key moments) */
	UFUNCTION(BlueprintCallable, Category = "SOD|Camera")
	void TriggerHeroShot(FVector FocusPoint, float Duration);

protected:
	virtual void BeginPlay() override;

	/** Distance at which cameras merge into shared view */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SOD|Camera")
	float MergeDistance;

	/** Distance at which cameras split */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SOD|Camera")
	float SplitDistance;

	/** Blend speed for transitions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SOD|Camera")
	float BlendSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|Camera")
	ESODSplitScreenMode CurrentMode;

private:
	void UpdateDynamicSplit();
	float GetPlayerDistance() const;
	TArray<ASODPlayerCharacter*> GetPlayers() const;
};
