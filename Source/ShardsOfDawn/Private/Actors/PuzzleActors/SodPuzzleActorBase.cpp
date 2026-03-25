// Copyright Shards of Dawn Team 2026

#include "Actors/PuzzleActors/SodPuzzleActorBase.h"
#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/RotatingMovementComponent.h"
#include "GameModes/SodGameMode.h"
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
    RequiredArchetype = EPlayerArchetype::Any;
    ShardID = NAME_None;
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
    if (bRequiresBothArchetypes && !HasBothArchetypesReady(Interactor))
    {
        return false;
    }

    // Check archetype requirement
    if (RequiredArchetype != EPlayerArchetype::Any && Interactor->Archetype != RequiredArchetype)
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
        if (!HasBothArchetypesReady(Interactor))
        {
            BP_OnInteractionFailed(Interactor, FText::FromString(TEXT("Both archetypes must be present.")));
            PlaySoundAtLocation(InteractionAttemptSound);
            return;
        }
    }

    if (RequiredArchetype != EPlayerArchetype::Any && Interactor->Archetype != RequiredArchetype)
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
    ApplyActivationState(bActivated, Interactor);
}

void ASodPuzzleActorBase::OnRep_IsActivated()
{
    bIsActivated = RepIsActivated;
    HandleActivationStateChanged(bIsActivated);
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

void ASodPuzzleActorBase::ResetPuzzleState()
{
    ApplyActivationState(false, nullptr);
}

void ASodPuzzleActorBase::HandleActivationStateChanged(bool bActivated)
{
    if (bActivated)
    {
        ApplyMaterial(ActiveMaterial.IsNull() ? nullptr : ActiveMaterial.LoadSynchronous());
        if (VFXComp && !VFXComp->IsActive())
        {
            VFXComp->Activate(true);
        }
    }
    else
    {
        ApplyMaterial(InactiveMaterial.IsNull() ? nullptr : InactiveMaterial.LoadSynchronous());
        if (VFXComp)
        {
            VFXComp->Deactivate();
        }
    }
}

bool ASodPuzzleActorBase::HasBothArchetypesReady(ASodPlayerCharacter* Interactor) const
{
    bool bHasLightWeaver = Interactor && Interactor->Archetype == EPlayerArchetype::LightWeaver;
    bool bHasShadowWalker = Interactor && Interactor->Archetype == EPlayerArchetype::ShadowWalker;

    if (InteractionZone)
    {
        TArray<AActor*> OverlappingActors;
        InteractionZone->GetOverlappingActors(OverlappingActors, ASodPlayerCharacter::StaticClass());

        for (AActor* Actor : OverlappingActors)
        {
            if (const ASodPlayerCharacter* PlayerCharacter = Cast<ASodPlayerCharacter>(Actor))
            {
                bHasLightWeaver |= PlayerCharacter->Archetype == EPlayerArchetype::LightWeaver;
                bHasShadowWalker |= PlayerCharacter->Archetype == EPlayerArchetype::ShadowWalker;
            }
        }
    }

    return bHasLightWeaver && bHasShadowWalker;
}

void ASodPuzzleActorBase::ApplyActivationState(bool bActivated, ASodPlayerCharacter* Interactor)
{
    RepIsActivated = bActivated;
    bIsActivated = bActivated;

    HandleActivationStateChanged(bActivated);

    if (ASodGameMode* GameMode = GetWorld() ? Cast<ASodGameMode>(UGameplayStatics::GetGameMode(this)) : nullptr)
    {
        GameMode->SetShardActivationState(ShardID, bActivated);
    }

    if (bActivated)
    {
        BP_OnActivated(Interactor);
        PlaySoundAtLocation(ActivationSound.IsNull() ? nullptr : ActivationSound.LoadSynchronous());
    }
    else
    {
        BP_OnDeactivated(Interactor);
    }
}
