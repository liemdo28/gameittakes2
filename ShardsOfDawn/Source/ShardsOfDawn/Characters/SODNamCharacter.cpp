// Copyright Shards of Dawn. All Rights Reserved.

#include "Characters/SODNamCharacter.h"

ASODNamCharacter::ASODNamCharacter()
	: ShadowAnchorRange(1200.0f)
	, ShadowAnchorMaxDuration(8.0f)
	, AnchoredActor(nullptr)
	, AnchorTimer(0.0f)
{
	PlayerRole = ESODPlayerRole::Shadow;
}

void ASODNamCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASODNamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AnchoredActor)
	{
		AnchorTimer += DeltaTime;

		// Check if anchor duration expired
		if (AnchorTimer >= ShadowAnchorMaxDuration)
		{
			ReleaseShadowAnchor();
			return;
		}

		// Check if target moved out of range
		float Distance = FVector::Dist(GetActorLocation(), AnchoredActor->GetActorLocation());
		if (Distance > ShadowAnchorRange * 1.5f)
		{
			ReleaseShadowAnchor();
		}
	}
}

void ASODNamCharacter::ActivateShadowAnchor(AActor* Target)
{
	if (!Target || AnchoredActor)
	{
		return;
	}

	float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	if (Distance > ShadowAnchorRange)
	{
		return;
	}

	AnchoredActor = Target;
	AnchorTimer = 0.0f;

	// Lock the target's physics/state
	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Target->GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(false);
	}
}

void ASODNamCharacter::ReleaseShadowAnchor()
{
	if (AnchoredActor)
	{
		// Unlock the target's physics/state
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(AnchoredActor->GetRootComponent()))
		{
			PrimComp->SetSimulatePhysics(true);
		}

		AnchoredActor = nullptr;
		AnchorTimer = 0.0f;
	}
}
