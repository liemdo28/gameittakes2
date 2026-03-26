// Copyright Shards of Dawn Team 2026

#include "Actors/PuzzleActors/LightShardPuzzleActor.h"
#include "Characters/SODPlayerCharacter.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"

ALightShardPuzzleActor::ALightShardPuzzleActor()
{
    RequiredRole = ESODPuzzleRoleRequirement::Light;
    PuzzleName = FText::FromString(TEXT("Light Shard"));

    // Slightly slower rotation for Light shard
    if (RotateComp)
    {
        RotateComp->RotationRate = FRotator(0.0f, 45.0f, 0.0f);
    }
}

void ALightShardPuzzleActor::BeginPlay()
{
    Super::BeginPlay();
    InitialZ = GetActorLocation().Z;

    // Add point light
    PointLight = NewObject<UPointLightComponent>(this, TEXT("PointLight"));
    PointLight->SetupAttachment(RootComponent);
    PointLight->SetLightColor(LightColor);
    PointLight->SetIntensity(LightIntensity);
    PointLight->SetAttenuationRadius(LightRadius);
    PointLight->SetSourceRadius(30.0f);
    PointLight->SetFalloffExponent(2.0f);
    PointLight->SetSourceLength(0.0f);
    PointLight->bAutoActivate = false;
    PointLight->RegisterComponent();
    HandleActivationStateChanged(bIsActivated);
}

bool ALightShardPuzzleActor::CanInteract_Implementation(ASODPlayerCharacter* Interactor) const
{
    return Interactor && Interactor->GetPlayerRole() == ESODPlayerRole::Light;
}

void ALightShardPuzzleActor::OnInteract_Implementation(ASODPlayerCharacter* Interactor)
{
    if (!CanInteract_Implementation(Interactor))
    {
        BP_OnInteractionFailed(Interactor,
            FText::FromString(TEXT("Only Linh (Light) can activate this shard.")));
        return;
    }
    Super::OnInteract_Implementation(Interactor);
}

void ALightShardPuzzleActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Floating animation when active
    if (bIsActivated)
    {
        TimeAccum += DeltaSeconds;
        FVector Pos = GetActorLocation();
        Pos.Z = InitialZ + FMath::Sin(TimeAccum * FloatSpeed) * FloatAmplitude;
        SetActorLocation(Pos, true);
    }
}

void ALightShardPuzzleActor::HandleActivationStateChanged(bool bActivated)
{
    Super::HandleActivationStateChanged(bActivated);

    if (PointLight)
    {
        PointLight->SetVisibility(bActivated);
        PointLight->SetActive(bActivated);
    }
}
