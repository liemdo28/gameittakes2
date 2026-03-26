// Copyright Shards of Dawn Team 2026

#include "Actors/PuzzleActors/SodPuzzleActorBase.h"
#include "Characters/SODPlayerCharacter.h"
#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/RotatingMovementComponent.h"
#include "Core/SODGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

namespace
{
bool MatchesRequiredRole(const ASODPlayerCharacter* Interactor, ESODPuzzleRoleRequirement RequiredRole)
{
    if (!Interactor || RequiredRole == ESODPuzzleRoleRequirement::Any)
    {
        return Interactor != nullptr;
    }

    return (RequiredRole == ESODPuzzleRoleRequirement::Light && Interactor->GetPlayerRole() == ESODPlayerRole::Light)
        || (RequiredRole == ESODPuzzleRoleRequirement::Shadow && Interactor->GetPlayerRole() == ESODPlayerRole::Shadow);
}
}

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
    RequiredRole = ESODPuzzleRoleRequirement::Any;
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

bool ASodPuzzleActorBase::CanInteract_Implementation(ASODPlayerCharacter* Interactor) const
{
    if (!Interactor) return false;
    if (bRequiresBothArchetypes && !HasBothRolesReady(Interactor))
    {
        return false;
    }

    if (!MatchesRequiredRole(Interactor, RequiredRole))
    {
        return false;
    }
    return true;
}

void ASodPuzzleActorBase::OnInteract_Implementation(ASODPlayerCharacter* Interactor)
{
    if (!Interactor) return;

    if (bRequiresBothArchetypes)
    {
        if (!HasBothRolesReady(Interactor))
        {
            BP_OnInteractionFailed(Interactor, FText::FromString(TEXT("Both co-op roles must be present.")));
            PlaySoundAtLocation(InteractionAttemptSound);
            return;
        }
    }

    if (!MatchesRequiredRole(Interactor, RequiredRole))
    {
        const FText FailureReason = (RequiredRole == ESODPuzzleRoleRequirement::Light)
            ? FText::FromString(TEXT("Only Linh (Light) can activate this."))
            : FText::FromString(TEXT("Only Nam (Shadow) can activate this."));
        BP_OnInteractionFailed(Interactor, FailureReason);
        PlaySoundAtLocation(InteractionAttemptSound);
        return;
    }

    // Toggle activation state
    Server_SetActivated(!RepIsActivated, Interactor);
}

void ASodPuzzleActorBase::Server_SetActivated_Implementation(bool bActivated, ASODPlayerCharacter* Interactor)
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
    // Called by ASODGameMode::ResetAllPuzzles() via IInterface_Interactive
    IInterface_Interactive::ResetPuzzleState_Implementation();
}

void ASodPuzzleActorBase::ResetPuzzleState_Implementation()
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

bool ASodPuzzleActorBase::HasBothRolesReady(ASODPlayerCharacter* Interactor) const
{
    bool bHasLightRole = Interactor && Interactor->GetPlayerRole() == ESODPlayerRole::Light;
    bool bHasShadowRole = Interactor && Interactor->GetPlayerRole() == ESODPlayerRole::Shadow;

    if (InteractionZone)
    {
        TArray<AActor*> OverlappingActors;
        InteractionZone->GetOverlappingActors(OverlappingActors, ASODPlayerCharacter::StaticClass());

        for (AActor* Actor : OverlappingActors)
        {
            if (const ASODPlayerCharacter* PlayerCharacter = Cast<ASODPlayerCharacter>(Actor))
            {
                bHasLightRole |= PlayerCharacter->GetPlayerRole() == ESODPlayerRole::Light;
                bHasShadowRole |= PlayerCharacter->GetPlayerRole() == ESODPlayerRole::Shadow;
            }
        }
    }

    return bHasLightRole && bHasShadowRole;
}

void ASodPuzzleActorBase::ApplyActivationState(bool bActivated, ASODPlayerCharacter* Interactor)
{
    RepIsActivated = bActivated;
    bIsActivated = bActivated;

    HandleActivationStateChanged(bActivated);

    if (ASODGameMode* GameMode = GetWorld() ? Cast<ASODGameMode>(UGameplayStatics::GetGameMode(this)) : nullptr)
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
