// Copyright Shards of Dawn. All Rights Reserved.

#include "Core/SODGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

USODGameInstance::USODGameInstance()
	: CurrentChapter(0)
	, CoOpSyncScore(0.5f)
{
}

void USODGameInstance::Init()
{
	Super::Init();
}

void USODGameInstance::HostCoOpSession(bool bIsLAN)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
	{
		UE_LOG(LogTemp, Warning, TEXT("SOD: No online subsystem found, falling back to local play"));
		return;
	}

	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid())
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = bIsLAN;
	SessionSettings.NumPublicConnections = 1; // Co-op: exactly 2 players
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(FName("GameMode"), FString("CoOp"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	Sessions->CreateSession(0, NAME_GameSession, SessionSettings);
}

void USODGameInstance::JoinCoOpSession(const FString& Address)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(Address, TRAVEL_Absolute);
	}
}

void USODGameInstance::AddCoOpSyncScore(float Delta)
{
	CoOpSyncScore = FMath::Clamp(CoOpSyncScore + Delta, 0.0f, 1.0f);
}
