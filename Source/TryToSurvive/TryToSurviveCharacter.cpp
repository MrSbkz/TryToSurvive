#include "TryToSurviveCharacter.h"

ATryToSurviveCharacter::ATryToSurviveCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	Mesh = GetMesh();
	Mesh->SetRelativeLocation(FVector(0.0f, 0.f, -95.0f));

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetRelativeLocation(FVector(22.0f, 0.f, 70.f)); // Position the camera

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(FirstPersonCameraComponent);

	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	BuildingComponent = CreateDefaultSubobject<UBuildingComponent>("BuildingComponent");
}

void ATryToSurviveCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);

			ThirdPersonCameraComponent->Activate();
			FirstPersonCameraComponent->Deactivate();

			this->bUseControllerRotationYaw = false;
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
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &ATryToSurviveCharacter::OnToggleCamera);
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

void ATryToSurviveCharacter::Chop()
{

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ChopAnimation);
		UGameplayStatics::ApplyDamage(HarvestActor[0], 40.0f, nullptr, this, UDamageType::StaticClass());
	}
}

void ATryToSurviveCharacter::OnSwitchBuildMode()
{
	if (!BuildingComponent->BuildingMenu)
	{
		BuildingComponent->CreateBuildingMenu();
	}

	if (BuildingComponent->BuildingMenu && !BuildingComponent->BuildingMenu->SetIgnorePlayerMovement.IsBound())
	{
		BuildingComponent->BuildingMenu->SetIgnorePlayerMovement.AddDynamic(this, &ATryToSurviveCharacter::SetIgnorePlayerMovement);
	}
	BuildingComponent->SetBuildingMode();
}

void ATryToSurviveCharacter::OnHit()
{
	if (BuildingComponent->IsBuildingMode)
	{
		BuildingComponent->Build();
	}
	else
	{
		if(isEnabledAttack)
		{
			switch (AttackState)
			{
			case EAttackState::Gather:
				Chop();
				break;
			case EAttackState::Attack:
				break;
			}
		}
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

void ATryToSurviveCharacter::OnToggleCamera()
{
	if(ThirdPersonCameraComponent->IsActive())
	{
		ThirdPersonCameraComponent->Deactivate();
		FirstPersonCameraComponent->Activate();

		this->bUseControllerRotationYaw = true;
	}
	else
	{
		FirstPersonCameraComponent->Deactivate();
		ThirdPersonCameraComponent->Activate();

		this->bUseControllerRotationYaw = false;
	}
}

void ATryToSurviveCharacter::SetIgnorePlayerMovement(bool IsEnabled)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (IsEnabled)
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(BuildingComponent->BuildingMenu->TakeWidget());
			PlayerController->SetInputMode(InputMode);
		}
		else
		{
			const FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}
		PlayerController->SetIgnoreLookInput(IsEnabled);
		PlayerController->SetIgnoreMoveInput(IsEnabled);
		PlayerController->SetShowMouseCursor(IsEnabled);
	}
}
