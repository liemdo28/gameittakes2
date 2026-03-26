// Copyright Shards of Dawn. All Rights Reserved.

#include "Core/SODGameInstance.h"
#include "EnhancedInputSubsystems.h"
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
	UE_LOG(LogTemp, Log, TEXT("[USODGameInstance] Initialized — Shards of Dawn"));
}

void USODGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Log, TEXT("[USODGameInstance] Shutdown"));
	Super::Shutdown();
}

void USODGameInstance::HostCoOpSession(bool bIsLAN)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (!OnlineSub)
	{
		UE_LOG(LogTemp, Warning, TEXT("[USODGameInstance] No online subsystem — falling back to local play"));
		return;
	}

	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
	if (!Sessions.IsValid()) return;

	FOnlineSessionSettings Settings;
	Settings.bIsLANMatch = bIsLAN;
	Settings.NumPublicConnections = 2; // Co-op: 2 players
	Settings.bShouldAdvertise = true;
	Settings.bUsesPresence = true;
	Settings.Set(FName("GameMode"), FString("CoOp"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	Sessions->CreateSession(0, NAME_GameSession, Settings);
}

void USODGameInstance::JoinCoOpSession(const FString& Address)
{
	if (APlayerController* PC = GetFirstLocalPlayerController())
	{
		PC->ClientTravel(Address, TRAVEL_Absolute);
	}
}

void USODGameInstance::AdvanceChapter()
{
	CurrentChapter++;
	UE_LOG(LogTemp, Log, TEXT("[USODGameInstance] Advanced to chapter %d"), CurrentChapter);
}

void USODGameInstance::ResetProgress()
{
	CurrentChapter = 0;
	CoOpSyncScore = 0.5f;
	SaveSlotName = TEXT("Slot_AutoSave");
	UE_LOG(LogTemp, Log, TEXT("[USODGameInstance] Progress reset."));
}

void USODGameInstance::AddCoOpSyncScore(float Delta)
{
	CoOpSyncScore = FMath::Clamp(CoOpSyncScore + Delta, 0.0f, 1.0f);
}

void USODGameInstance::ApplyIMC(int32 PlayerIndex, UInputMappingContext* IMC)
{
	if (!IMC) return;
	if (ULocalPlayer* LP = GetLocalPlayers().IsValidIndex(PlayerIndex) ? GetLocalPlayers()[PlayerIndex] : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
		{
			Sub->AddMappingContext(IMC, 0);
		}
	}
}

void USODGameInstance::RemoveIMC(int32 PlayerIndex, UInputMappingContext* IMC)
{
	if (!IMC) return;
	if (ULocalPlayer* LP = GetLocalPlayers().IsValidIndex(PlayerIndex) ? GetLocalPlayers()[PlayerIndex] : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
		{
			Sub->RemoveMappingContext(IMC);
		}
	}
}

void USODGameInstance::LogGameEvent(const FString& EventName, const TMap<FString, FString>& Params)
{
	FString ParamStr;
	for (const auto& KV : Params)
	{
		ParamStr += FString::Printf(TEXT(" %s=%s"), *KV.Key, *KV.Value);
	}
	UE_LOG(LogTemp, Log, TEXT("[TELEMETRY] Event=%s%s"), *EventName, *ParamStr);
}
