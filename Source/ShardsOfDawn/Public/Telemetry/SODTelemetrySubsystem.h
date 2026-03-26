// Copyright Shards of Dawn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SODTelemetrySubsystem.generated.h"

/**
 * Telemetry subsystem for tracking tester metrics.
 * Logs all gameplay events to Output Log for QA analysis.
 *
 * Events tracked:
 *   - Session start/end
 *   - Puzzle attempts, solves, timeouts
 *   - Combat hits, shield breaks, finishers
 *   - Player deaths, revives
 *   - Co-op pings
 *   - Level completion stats
 */
UCLASS()
class SHARDSOFDAWN_API USODTelemetrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Log a gameplay event */
	UFUNCTION(BlueprintCallable, Category = "SOD|Telemetry")
	void LogEvent(const FString& Category, const FString& EventName, const FString& Details = TEXT(""));

	/** Log a timed event (records duration) */
	UFUNCTION(BlueprintCallable, Category = "SOD|Telemetry")
	void StartTimedEvent(const FString& EventID);

	UFUNCTION(BlueprintCallable, Category = "SOD|Telemetry")
	void EndTimedEvent(const FString& EventID, const FString& Result = TEXT("Success"));

	/** Get total event count */
	UFUNCTION(BlueprintPure, Category = "SOD|Telemetry")
	int32 GetTotalEventCount() const { return TotalEvents; }

	/** Print full session summary to log */
	UFUNCTION(BlueprintCallable, Category = "SOD|Telemetry")
	void PrintSessionSummary();

private:
	struct FTimedEvent
	{
		FString EventID;
		float StartTime;
	};

	TMap<FString, FTimedEvent> ActiveTimedEvents;
	TMap<FString, int32> EventCounts;
	int32 TotalEvents;
	float SessionStartTime;
};
