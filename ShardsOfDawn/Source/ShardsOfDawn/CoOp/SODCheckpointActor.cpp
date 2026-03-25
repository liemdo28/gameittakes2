// Copyright Shards of Dawn. All Rights Reserved.

#include "CoOp/SODCheckpointActor.h"
#include "Characters/SODPlayerCharacter.h"
#include "Core/SODGameMode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ASODCheckpointActor::ASODCheckpointActor()
	: bRequireBothPlayers(true)
	, bActivated(false)
	, PlayersInVolume(0)
{
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	SetRootComponent(TriggerVolume);
	TriggerVolume->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	TriggerVolume->SetGenerateOverlapEvents(true);
}

void ASODCheckpointActor::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ASODCheckpointActor::OnOverlapBegin);
}

void ASODCheckpointActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActivated)
	{
		return;
	}

	ASODPlayerCharacter* Player = Cast<ASODPlayerCharacter>(OtherActor);
	if (!Player)
	{
		return;
	}

	PlayersInVolume++;

	if (bRequireBothPlayers && PlayersInVolume >= 2)
	{
		ActivateCheckpoint();
	}
	else if (!bRequireBothPlayers)
	{
		ActivateCheckpoint();
	}
}

void ASODCheckpointActor::ActivateCheckpoint()
{
	bActivated = true;

	ASODGameMode* GM = Cast<ASODGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->SetCheckpoint(GetActorTransform());
	}

	UE_LOG(LogTemp, Log, TEXT("SOD: Checkpoint activated at %s"), *GetActorLocation().ToString());
}
