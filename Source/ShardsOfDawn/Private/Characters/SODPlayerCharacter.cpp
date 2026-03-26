// Copyright Shards of Dawn. All Rights Reserved.

#include "Characters/SODPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ASODPlayerCharacter::ASODPlayerCharacter()
	: PlayerRole(ESODPlayerRole::Light)
	, bIsDown(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Movement defaults for platformer-style co-op
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
}

UAbilitySystemComponent* ASODPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

ASODPlayerCharacter* ASODPlayerCharacter::GetPartner() const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() != this)
		{
			return Cast<ASODPlayerCharacter>(PC->GetPawn());
		}
	}
	return nullptr;
}

void ASODPlayerCharacter::ServerPing_Implementation(FVector PingLocation)
{
	ASODPlayerCharacter* Partner = GetPartner();
	if (Partner)
	{
		Partner->OnPartnerPinged(PingLocation);
	}
}

void ASODPlayerCharacter::ServerRevivePartner_Implementation()
{
	ASODPlayerCharacter* Partner = GetPartner();
	if (Partner && Partner->bIsDown)
	{
		Partner->bIsDown = false;
		// TODO: Play revive animation, restore health
	}
}

void ASODPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add starting gameplay tags
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddLooseGameplayTags(StartingTags);
	}
}

void ASODPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server: grant starting abilities
	if (AbilitySystemComponent && HasAuthority())
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartingAbilities)
		{
			if (AbilityClass)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
			}
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// Setup input mapping context
	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ASODPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (MoveAction)
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASODPlayerCharacter::Move);
	if (LookAction)
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASODPlayerCharacter::Look);
	if (JumpAction)
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ASODPlayerCharacter::JumpPressed);
	if (InteractAction)
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ASODPlayerCharacter::InteractPressed);
	if (AbilityPrimaryAction)
		EnhancedInput->BindAction(AbilityPrimaryAction, ETriggerEvent::Started, this, &ASODPlayerCharacter::AbilityPrimaryPressed);
	if (AbilitySecondaryAction)
		EnhancedInput->BindAction(AbilitySecondaryAction, ETriggerEvent::Started, this, &ASODPlayerCharacter::AbilitySecondaryPressed);
	if (PingAction)
		EnhancedInput->BindAction(PingAction, ETriggerEvent::Started, this, &ASODPlayerCharacter::PingPressed);
}

void ASODPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASODPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASODPlayerCharacter::JumpPressed()
{
	Jump();
}

void ASODPlayerCharacter::InteractPressed()
{
	// Interaction handled via GAS ability activation
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer InteractTag;
		InteractTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Interact")));
		AbilitySystemComponent->TryActivateAbilitiesByTag(InteractTag);
	}
}

void ASODPlayerCharacter::AbilityPrimaryPressed()
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer PrimaryTag;
		PrimaryTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Primary")));
		AbilitySystemComponent->TryActivateAbilitiesByTag(PrimaryTag);
	}
}

void ASODPlayerCharacter::AbilitySecondaryPressed()
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer SecondaryTag;
		SecondaryTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Secondary")));
		AbilitySystemComponent->TryActivateAbilitiesByTag(SecondaryTag);
	}
}

void ASODPlayerCharacter::PingPressed()
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		if (HitResult.bBlockingHit)
		{
			ServerPing(HitResult.ImpactPoint);
		}
	}
}

void ASODPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASODPlayerCharacter, PlayerRole);
	DOREPLIFETIME(ASODPlayerCharacter, bIsDown);
}

void ASODPlayerCharacter::SetPlayerRole_Implementation(ESODPlayerRole NewRole)
{
	PlayerRole = NewRole;
	UE_LOG(LogTemp, Log, TEXT("[SODPlayerCharacter] Role set to: %s"),
		NewRole == ESODPlayerRole::Light ? TEXT("Light") : TEXT("Shadow"));
}

void ASODPlayerCharacter::OnRep_PlayerRole()
{
	UE_LOG(LogTemp, Log, TEXT("[SODPlayerCharacter] Role replicated: %s"),
		PlayerRole == ESODPlayerRole::Light ? TEXT("Light") : TEXT("Shadow"));

	// Refresh controller IMC so input maps update on role change
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (PC->IsLocalController())
		{
			PC->ConsoleCommand(TEXT("refreshimc"), true);
		}
	}
}
