// Copyright Shards of Dawn Team 2026

#include "Actors/PuzzleActors/SodPuzzleActorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/NiagaraComponent.h"
#include "Components/RotatingMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASodPuzzleActorBase::ASodPuzzleActorBase()
{
    bReplicates = true;
    SetReplicateMovement(false);

    // ── Root ─────────────────────────────────────────────────────
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // ── Mesh ──────────────────────────────────────────────────────
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
    MeshComp->SetGenerateOverlapEvents(true);
    MeshComp->SetIsReplicated(true);

    // ── Interaction Zone ──────────────────────────────────────────
    InteractionZone = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionZone"));
    InteractionZone->SetupAttachment(RootComponent);
    InteractionZone->SetSphereRadius(120.0f);
    InteractionZone->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    InteractionZone->SetGenerateOverlapEvents(true);

    // ── VFX ───────────────────────────────────────────────────────
    VFXComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFXComp"));
    VFXComp->SetupAttachment(RootComponent);
    VFXComp->bAutoActivate = false;

    // ── Rotation ──────────────────────────────────────────────────
    RotateComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateComp"));
    RotateComp->RotationRate = FRotator(0.0f, 90.0f, 0.0f);
    RotateComp->bRotationInLocalSpace = true;

    // ── Defaults ──────────────────────────────────────────────────
    PrimaryActorTick.bCanEverTick = true;
    PuzzleName = FText::FromString(TEXT("Unknown Puzzle"));
    RequiredArchetype = 0; // Either
}

void ASodPuzzleActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASodPuzzleActorBase, RepIsActivated);
}

// ── IInterface_Interactive ────────────────────────────────────────────────────

FText ASodPuzzleActorBase::GetInteractionPrompt_Implementation() const
{
    if (bIsActivated)
    {
        return FText::FromString(TEXT("[E] Deactivate"));
    }
    if (bRequiresBothArchetypes)
    {
        return FText::FromString(TEXT("[E] Requires Both Players"));
    }
    return FText::FromString(TEXT("[E] Interact"));
}

bool ASodPuzzleActorBase::CanInteract_Implementation(ASodPlayerCharacter* Interactor) const
{
    if (!Interactor) return false;
    if (bRequiresBothArchetypes) return false; // Handled by game mode

    // Check archetype requirement
    if (RequiredArchetype != 0 && Interactor->Archetype != RequiredArchetype)
    {
        return false;
    }
    return true;
}

void ASodPuzzleActorBase::OnInteract_Implementation(ASodPlayerCharacter* Interactor)
{
    if (!Interactor) return;

    if (bRequiresBothArchetypes)
    {
        BP_OnInteractionFailed(Interactor, FText::FromString(TEXT("Requires both players!")));
        PlaySoundAtLocation(InteractionAttemptSound);
        return;
    }

    if (RequiredArchetype != 0 && Interactor->Archetype != RequiredArchetype)
    {
        BP_OnInteractionFailed(Interactor,
            FText::FromString(Interactor->Archetype == EPlayerArchetype::LightWeaver
                ? TEXT("Light Weaver cannot activate this.")
                : TEXT("Shadow Walker cannot activate this.")));
        PlaySoundAtLocation(InteractionAttemptSound);
        return;
    }

    // Toggle activation state
    Server_SetActivated(!RepIsActivated, Interactor);
}

void ASodPuzzleActorBase::Server_SetActivated_Implementation(bool bActivated, ASodPlayerCharacter* Interactor)
{
    RepIsActivated = bActivated;
    bIsActivated = bActivated;

    if (bActivated)
    {
        BP_OnActivated(Interactor);
        ApplyMaterial(ActiveMaterial.Get());
        VFXComp->Activate(true);
        PlaySoundAtLocation(ActivationSound);
    }
    else
    {
        BP_OnDeactivated(Interactor);
        ApplyMaterial(InactiveMaterial.Get());
        VFXComp->Deactivate();
    }
}

void ASodPuzzleActorBase::OnRep_IsActivated()
{
    bIsActivated = RepIsActivated;
    if (bIsActivated)
    {
        ApplyMaterial(ActiveMaterial.Get());
        if (VFXComp && !VFXComp->IsActive()) VFXComp->Activate(true);
    }
    else
    {
        ApplyMaterial(InactiveMaterial.Get());
        if (VFXComp) VFXComp->Deactivate();
    }
}

// ── Helpers ───────────────────────────────────────────────────────────────────

void ASodPuzzleActorBase::ApplyMaterial(UMaterialInterface* Material)
{
    if (Material && MeshComp)
    {
        MeshComp->SetMaterial(0, Material);
    }
}

void ASodPuzzleActorBase::PlaySoundAtLocation(USoundBase* Sound)
{
    if (Sound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation(), FRotator::ZeroRotator);
    }
}
