// Copyright Shards of Dawn. All Rights Reserved.

#include "Telemetry/SODTelemetrySubsystem.h"
#include "Kismet/GameplayStatics.h"

void USODTelemetrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	TotalEvents = 0;
	SessionStartTime = FPlatformTime::Seconds();

	UE_LOG(LogTemp, Warning, TEXT("[TELEMETRY] Session started at %.2f"), SessionStartTime);
	LogEvent(TEXT("Session"), TEXT("Start"), TEXT("Shards of Dawn Prototype"));
}

void USODTelemetrySubsystem::Deinitialize()
{
	PrintSessionSummary();
	Super::Deinitialize();
}

void USODTelemetrySubsystem::LogEvent(const FString& Category, const FString& EventName, const FString& Details)
{
	TotalEvents++;

	FString Key = FString::Printf(TEXT("%s.%s"), *Category, *EventName);
	int32& Count = EventCounts.FindOrAdd(Key);
	Count++;

	float Elapsed = FPlatformTime::Seconds() - SessionStartTime;

	UE_LOG(LogTemp, Log, TEXT("[TELEMETRY] [%.1fs] %s | %s | %s (#%d)"),
		Elapsed, *Category, *EventName, *Details, Count);
}

void USODTelemetrySubsystem::StartTimedEvent(const FString& EventID)
{
	FTimedEvent NewEvent;
	NewEvent.EventID = EventID;
	NewEvent.StartTime = FPlatformTime::Seconds();
	ActiveTimedEvents.Add(EventID, NewEvent);

	LogEvent(TEXT("Timer"), TEXT("Start"), EventID);
}

void USODTelemetrySubsystem::EndTimedEvent(const FString& EventID, const FString& Result)
{
	FTimedEvent* Event = ActiveTimedEvents.Find(EventID);
	if (Event)
	{
		float Duration = FPlatformTime::Seconds() - Event->StartTime;
		FString Details = FString::Printf(TEXT("%s (%.1fs) - %s"), *EventID, Duration, *Result);
		LogEvent(TEXT("Timer"), TEXT("End"), Details);
		ActiveTimedEvents.Remove(EventID);
	}
}

void USODTelemetrySubsystem::PrintSessionSummary()
{
	float SessionDuration = FPlatformTime::Seconds() - SessionStartTime;

	UE_LOG(LogTemp, Warning, TEXT(""));
	UE_LOG(LogTemp, Warning, TEXT("========== TELEMETRY SESSION SUMMARY =========="));
	UE_LOG(LogTemp, Warning, TEXT("  Duration: %.1f seconds (%.1f minutes)"), SessionDuration, SessionDuration / 60.0f);
	UE_LOG(LogTemp, Warning, TEXT("  Total Events: %d"), TotalEvents);
	UE_LOG(LogTemp, Warning, TEXT(""));

	// Sort and print event counts
	EventCounts.ValueSort([](const int32& A, const int32& B) { return A > B; });

	for (const auto& Pair : EventCounts)
	{
		UE_LOG(LogTemp, Warning, TEXT("  %s: %d"), *Pair.Key, Pair.Value);
	}

	UE_LOG(LogTemp, Warning, TEXT("================================================"));
}
