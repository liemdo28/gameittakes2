// Copyright Shards of Dawn. All Rights Reserved.

#include "CoOp/SODCoOpSyncSubsystem.h"

void USODCoOpSyncSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SyncScore = 0.5f; // Start at neutral
	TotalSuccesses = 0;
	TotalFailures = 0;
}

void USODCoOpSyncSubsystem::RecordCoOpSuccess(float Weight)
{
	SyncScore = FMath::Clamp(SyncScore + (SuccessIncrement * Weight), 0.0f, 1.0f);
	TotalSuccesses++;
}

void USODCoOpSyncSubsystem::RecordCoOpFailure(float Weight)
{
	SyncScore = FMath::Clamp(SyncScore - (FailureDecrement * Weight), 0.0f, 1.0f);
	TotalFailures++;
}

float USODCoOpSyncSubsystem::GetDifficultyMultiplier() const
{
	// High sync -> slightly harder (reward skilled play)
	// Low sync -> slightly easier (prevent frustration)
	// Range: 0.8 (easiest) to 1.2 (hardest)
	return 0.8f + (SyncScore * 0.4f);
}

int32 USODCoOpSyncSubsystem::GetMusicIntensityLayer() const
{
	// 3-layer adaptive music system:
	// Layer 0 (Environment): SyncScore < 0.3 or idle
	// Layer 1 (Action): SyncScore 0.3 - 0.7
	// Layer 2 (Climax): SyncScore > 0.7
	if (SyncScore < 0.3f)
	{
		return 0;
	}
	else if (SyncScore < 0.7f)
	{
		return 1;
	}
	return 2;
}
