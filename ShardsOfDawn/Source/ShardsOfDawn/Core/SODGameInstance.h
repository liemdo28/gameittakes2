// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SODGameInstance.generated.h"

/**
 * Core game instance for Shards of Dawn.
 * Manages session lifecycle, player pairing, and persistent co-op state.
 */
UCLASS()
class SHARDSOFDAWN_API USODGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USODGameInstance();

	virtual void Init() override;

	/** Host a new co-op session (local split-screen or online) */
	UFUNCTION(BlueprintCallable, Category = "SOD|Session")
	void HostCoOpSession(bool bIsLAN);

	/** Join an existing co-op session */
	UFUNCTION(BlueprintCallable, Category = "SOD|Session")
	void JoinCoOpSession(const FString& Address);

	/** Get current chapter index (0-based) */
	UFUNCTION(BlueprintPure, Category = "SOD|Progress")
	int32 GetCurrentChapter() const { return CurrentChapter; }

	/** Get co-op sync score (0.0 - 1.0) */
	UFUNCTION(BlueprintPure, Category = "SOD|CoOp")
	float GetCoOpSyncScore() const { return CoOpSyncScore; }

	/** Add to the co-op sync score from successful cooperation */
	UFUNCTION(BlueprintCallable, Category = "SOD|CoOp")
	void AddCoOpSyncScore(float Delta);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "SOD|Progress")
	int32 CurrentChapter;

	/** Tracks how well the two players cooperate (drives adaptive music/VO) */
	UPROPERTY(BlueprintReadOnly, Category = "SOD|CoOp")
	float CoOpSyncScore;
};
