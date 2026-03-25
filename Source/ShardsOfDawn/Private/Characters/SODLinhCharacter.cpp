// Copyright Shards of Dawn. All Rights Reserved.

#include "Characters/SODLinhCharacter.h"
#include "Kismet/GameplayStatics.h"

ASODLinhCharacter::ASODLinhCharacter()
	: LightPulseRadius(800.0f)
	, LightPulseDuration(3.0f)
	, LightPulseCooldown(5.0f)
	, bLightPulseActive(false)
	, LightPulseTimer(0.0f)
	, LightPulseCooldownTimer(0.0f)
{
	PlayerRole = ESODPlayerRole::Light;
}

void ASODLinhCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASODLinhCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLightPulseActive)
	{
		LightPulseTimer -= DeltaTime;
		if (LightPulseTimer <= 0.0f)
		{
			bLightPulseActive = false;
			LightPulseCooldownTimer = LightPulseCooldown;
		}
	}
	else if (LightPulseCooldownTimer > 0.0f)
	{
		LightPulseCooldownTimer -= DeltaTime;
	}
}

void ASODLinhCharacter::ActivateLightPulse()
{
	if (bLightPulseActive || LightPulseCooldownTimer > 0.0f)
	{
		return;
	}

	bLightPulseActive = true;
	LightPulseTimer = LightPulseDuration;

	// Overlap check: find all actors in pulse radius that respond to light
	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(LightPulseRadius);

	GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation() + FVector(0, 0, 1),
		FQuat::Identity,
		ECC_GameTraceChannel1, // Custom "LightInteract" channel
		SphereShape
	);

	for (const FHitResult& Hit : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			// Notify actors that implement the light-reactive interface
			// This will reveal hidden platforms, shadow objects, etc.
			IInterface_LightReactive* Reactive = Cast<IInterface_LightReactive>(HitActor);
			if (Reactive)
			{
				// Interface call handled in puzzle/environment actors
			}
		}
	}
}
