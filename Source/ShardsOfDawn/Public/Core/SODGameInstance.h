// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SODGameInstance.generated.h"

class UInputMappingContext;

/**
 * USODGameInstance — CANONICAL GameInstance for Shards of Dawn.
 *
 * Responsibilities:
 * - Session lifecycle (host / join co-op)
 * - Cross-level persistence (chapter progress, co-op sync score)
 * - IMC management helpers
 * - Telemetry / event logging
 *
 * Architecture note: This is the SINGLE authoritative GameInstance class.
 */
UCLASS()
class SHARDSOFDAWN_API USODGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USODGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	// ── Session ─────────────────────────────────────────────────────────────
	/** Host a new co-op session (local split-screen or LAN). */
	UFUNCTION(BlueprintCallable, Category = "SOD|Session")
	void HostCoOpSession(bool bIsLAN);

	/** Join an existing co-op session by address. */
	UFUNCTION(BlueprintCallable, Category = "SOD|Session")
	void JoinCoOpSession(const FString& Address);

	// ── Progress ────────────────────────────────────────────────────────────
	UFUNCTION(BlueprintPure, Category = "SOD|Progress")
	int32 GetCurrentChapter() const { return CurrentChapter; }

	UFUNCTION(BlueprintCallable, Category = "SOD|Progress")
	void AdvanceChapter();

	UFUNCTION(BlueprintCallable, Category = "SOD|Progress")
	void ResetProgress();

	// ── Co-op Sync Score ──────────────────────────────────────────────────
	UFUNCTION(BlueprintPure, Category = "SOD|CoOp")
	float GetCoOpSyncScore() const { return CoOpSyncScore; }

	UFUNCTION(BlueprintCallable, Category = "SOD|CoOp")
	void AddCoOpSyncScore(float Delta);

	// ── IMC Management ───────────────────────────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "SOD|Input")
	void ApplyIMC(int32 PlayerIndex, UInputMappingContext* IMC);

	UFUNCTION(BlueprintCallable, Category = "SOD|Input")
	void RemoveIMC(int32 PlayerIndex, UInputMappingContext* IMC);

	// ── Telemetry ─────────────────────────────────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "SOD|Telemetry")
	void LogGameEvent(const FString& EventName, const TMap<FString, FString>& Params);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "SOD|Progress")
	int32 CurrentChapter = 0;

	UPROPERTY(BlueprintReadOnly, Category = "SOD|CoOp")
	float CoOpSyncScore = 0.5f;

	UPROPERTY(BlueprintReadWrite, Category = "SOD|Progress")
	FString SaveSlotName = TEXT("Slot_AutoSave");
};
