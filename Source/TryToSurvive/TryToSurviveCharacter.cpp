#include "TryToSurviveCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ATryToSurviveCharacter::ATryToSurviveCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	BuildingComponent = CreateDefaultSubobject<UBuildingComponent>("BuildingComponent");
}

void ATryToSurviveCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATryToSurviveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATryToSurviveCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATryToSurviveCharacter::Look);
		EnhancedInputComponent->BindAction(BuildModeAction, ETriggerEvent::Triggered, this, &ATryToSurviveCharacter::OnSwitchBuildMode);
		EnhancedInputComponent->BindAction(HitAction, ETriggerEvent::Triggered, this, &ATryToSurviveCharacter::OnHit);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATryToSurviveCharacter::OnRotationStart);
	}
}


void ATryToSurviveCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATryToSurviveCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATryToSurviveCharacter::OnSwitchBuildMode()
{
	BuildingComponent->SetBuildingMode();

	if(!BuildingComponent->IsBuildingMode)
	{
		BuildingComponent->ResetBuilding();
	}
}

void ATryToSurviveCharacter::OnHit()
{
	if(BuildingComponent->IsBuildingMode)
	{
		BuildingComponent->Build();
	}
}

void ATryToSurviveCharacter::OnRotationStart()
{
	BuildingComponent->ProcessRotation();
}

void ATryToSurviveCharacter::OnRotationComplete()
{
	BuildingComponent->ProcessRotation();
}
