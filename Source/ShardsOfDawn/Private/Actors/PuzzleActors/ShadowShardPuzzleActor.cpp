// Copyright Shards of Dawn Team 2026

#include "Actors/PuzzleActors/ShadowShardPuzzleActor.h"
#include "Characters/SODPlayerCharacter.h"
#include "Components/PointLightComponent.h"

AShadowShardPuzzleActor::AShadowShardPuzzleActor()
{
    RequiredRole = ESODPuzzleRoleRequirement::Shadow;
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
    VoidLight->SetAttenuationRadius(LightRadius);
    VoidLight->SetSourceRadius(30.0f);
    VoidLight->SetFalloffExponent(2.0f);
    VoidLight->bAutoActivate = false;
    VoidLight->RegisterComponent();
    HandleActivationStateChanged(bIsActivated);
}

bool AShadowShardPuzzleActor::CanInteract_Implementation(ASODPlayerCharacter* Interactor) const
{
    return Interactor && Interactor->GetPlayerRole() == ESODPlayerRole::Shadow;
}

void AShadowShardPuzzleActor::OnInteract_Implementation(ASODPlayerCharacter* Interactor)
{
    if (!CanInteract_Implementation(Interactor))
    {
        BP_OnInteractionFailed(Interactor,
            FText::FromString(TEXT("Only Nam (Shadow) can activate this shard.")));
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

void AShadowShardPuzzleActor::HandleActivationStateChanged(bool bActivated)
{
    Super::HandleActivationStateChanged(bActivated);

    if (VoidLight)
    {
        VoidLight->SetVisibility(bActivated);
        VoidLight->SetActive(bActivated);
    }
}
