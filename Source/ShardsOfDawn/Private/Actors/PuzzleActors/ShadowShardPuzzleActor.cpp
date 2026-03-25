// Copyright Shards of Dawn Team 2026

#include "Actors/PuzzleActors/ShadowShardPuzzleActor.h"
#include "Characters/SodPlayerCharacter.h"
#include "Components/PointLightComponent.h"

AShadowShardPuzzleActor::AShadowShardPuzzleActor()
{
    RequiredArchetype = EPlayerArchetype::ShadowWalker;
    PuzzleName = FText::FromString(TEXT("Shadow Shard"));

    if (RotateComp)
    {
        RotateComp->RotationRate = FRotator(0.0f, -60.0f, 0.0f); // opposite spin
    }
}

void AShadowShardPuzzleActor::BeginPlay()
{
    Super::BeginPlay();
    InitialZ = GetActorLocation().Z;

    VoidLight = NewObject<UPointLightComponent>(this, TEXT("VoidLight"));
    VoidLight->SetupAttachment(RootComponent);
    VoidLight->SetLightColor(LightColor);
    VoidLight->SetIntensity(LightIntensity);
    VoidLight->SetSourceRadius(30.0f);
    VoidLight->SetFalloffExponent(2.0f);
    VoidLight->bAutoActivate = false;
    VoidLight->RegisterComponent();
}

bool AShadowShardPuzzleActor::CanInteract_Implementation(ASodPlayerCharacter* Interactor) const
{
    return Interactor && Interactor->Archetype == EPlayerArchetype::ShadowWalker;
}

void AShadowShardPuzzleActor::OnInteract_Implementation(ASodPlayerCharacter* Interactor)
{
    if (!CanInteract_Implementation(Interactor))
    {
        BP_OnInteractionFailed(Interactor,
            FText::FromString(TEXT("Only Shadow Walker can activate this shard.")));
        return;
    }
    Super::OnInteract_Implementation(Interactor);
}

void AShadowShardPuzzleActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsActivated)
    {
        TimeAccum += DeltaSeconds;
        FVector Pos = GetActorLocation();
        // Void pulses downward
        Pos.Z = InitialZ + FMath::Sin(TimeAccum * PulseSpeed) * PulseAmplitude - 10.0f;
        SetActorLocation(Pos, true);
    }
}
