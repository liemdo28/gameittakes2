// Copyright Shards of Dawn Team 2026

#include "Characters/SodPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Controllers/SodPlayerController.h"
#include "Interfaces/Interface_Interactive.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Abilities/GameplayAbility.h"
#include "Animation/AnimInstance.h"

ASodPlayerCharacter::ASodPlayerCharacter()
{
    bReplicates = true;
    NetUpdateFrequency = 100.0f;

    // ── Capsule ──────────────────────────────────────────────────
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

    // ── Movement defaults ────────────────────────────────────────
    UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    MoveComp->bOrientRotationToMovement = false;
    MoveComp->bUseControllerDesiredRotation = true;
    MoveComp->JumpZVelocity = BaseJumpVelocity;
    MoveComp->MaxWalkSpeed = BaseMoveSpeed;
    MoveComp->MaxStepHeight = 50.0f;
    MoveComp->SetWalkableFloorAngle(56.0f);

    // ── Spring Arm ───────────────────────────────────────────────
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 400.0f;
    SpringArm->SocketOffset = FVector(0.0f, 0.0f, 60.0f);
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bInheritPitch = true;
    SpringArm->bInheritRoll = false;
    SpringArm->bInheritYaw = true;

    // ── Camera ───────────────────────────────────────────────────
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->FieldOfView = 90.0f;

    // ── Net ──────────────────────────────────────────────────────
    RepArchetype = Archetype;
}

void ASodPlayerCharacter::SetArchetype(EPlayerArchetype NewArchetype)
{
    Archetype = NewArchetype;
    RepArchetype = NewArchetype;

    if (GetLocalRole() != ROLE_SimulatedProxy)
    {
        BP_OnArchetypeChanged(NewArchetype);
    }

    if (ASodPlayerController* PlayerController = Cast<ASodPlayerController>(Controller))
    {
        PlayerController->RefreshInputMapping();
    }
}

void ASodPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASodPlayerCharacter, RepArchetype);
}

void ASodPlayerCharacter::OnRep_ArchetypeChanged()
{
    Archetype = RepArchetype;
    BP_OnArchetypeChanged(RepArchetype);

    if (ASodPlayerController* PlayerController = Cast<ASodPlayerController>(Controller))
    {
        PlayerController->RefreshInputMapping();
    }
}

FString ASodPlayerCharacter::GetDebugRoleString() const
{
    switch (GetLocalRole())
    {
    case ROLE_Authority:    return TEXT("Authority");
    case ROLE_AutonomousProxy: return TEXT("AutonomousProxy");
    case ROLE_SimulatedProxy:  return TEXT("SimulatedProxy");
    default:                 return TEXT("None");
    }
}

// ── Input ───────────────────────────────────────────────────────────────────

void ASodPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Movement — already wired via EnhancedInput plugin (see IMC below)
    // These are fallback Axis bindings for standard InputComponent
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASodPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"),   this, &ASodPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"),      this, &ASodPlayerCharacter::LookUp);
    PlayerInputComponent->BindAxis(TEXT("Turn"),        this, &ASodPlayerCharacter::Turn);

    PlayerInputComponent->BindAction(TEXT("Jump"),      IE_Pressed, this, &ASodPlayerCharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Jump"),      IE_Released, this, &ASodPlayerCharacter::StopJump);
    PlayerInputComponent->BindAction(TEXT("Interact"),  IE_Pressed, this, &ASodPlayerCharacter::TryInteract);
}

void ASodPlayerCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ASodPlayerCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ASodPlayerCharacter::LookUp(float Value)
{
    if (Controller)
    {
        AddControllerPitchInput(Value * 90.0f * GetWorld()->GetDeltaSeconds());
    }
}

void ASodPlayerCharacter::Turn(float Value)
{
    if (Controller)
    {
        AddControllerYawInput(Value * 90.0f * GetWorld()->GetDeltaSeconds());
    }
}

void ASodPlayerCharacter::Jump()
{
    Super::Jump();
}

void ASodPlayerCharacter::StopJump()
{
    Super::StopJumping();
}

// ── Interaction ──────────────────────────────────────────────────────────────

bool ASodPlayerCharacter::TraceForInteractable(FHitResult& OutHit) const
{
    if (!Controller) return false;

    FVector Loc;
    FRotator Rot;
    Controller->GetPlayerViewPoint(Loc, Rot);

    const FVector Start = Loc;
    const FVector End = Start + Rot.Vector() * InteractionReach;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    return GetWorld()->LineTraceSingleByChannel(
        OutHit, Start, End,
        InteractionTraceChannel,
        Params
    );
}

void ASodPlayerCharacter::TryInteract()
{
    FHitResult Hit;
    if (TraceForInteractable(Hit))
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor->GetClass()->ImplementsInterface(UInterface_Interactive::StaticClass()))
        {
            const bool bCanInteract = IInterface_Interactive::Execute_CanInteract(HitActor, this);
            if (bCanInteract)
            {
                if (HasAuthority())
                {
                    IInterface_Interactive::Execute_OnInteract(HitActor, this);
                }
                else
                {
                    ServerInteract(HitActor);
                }

                BP_OnInteract(HitActor);
            }

            if (bCanInteract && GetNetMode() != NM_DedicatedServer && InteractionMontage)
            {
                UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
                if (AnimInst)
                {
                    AnimInst->Montage_Play(InteractionMontage);
                }
            }
        }
    }

    // Debug sphere in editor
#if WITH_EDITOR
    DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * (InteractionReach * 0.5f),
        8.0f, 8, FColor::Cyan, false, 1.0f, 0, 1.0f);
#endif
}

void ASodPlayerCharacter::ServerInteract_Implementation(AActor* InteractableActor)
{
    if (!InteractableActor || !InteractableActor->GetClass()->ImplementsInterface(UInterface_Interactive::StaticClass()))
    {
        return;
    }

    const float MaxInteractDistance = FMath::Square(InteractionReach + 100.0f);
    if (FVector::DistSquared(GetActorLocation(), InteractableActor->GetActorLocation()) > MaxInteractDistance)
    {
        return;
    }

    if (IInterface_Interactive::Execute_CanInteract(InteractableActor, this))
    {
        IInterface_Interactive::Execute_OnInteract(InteractableActor, this);
    }
}
