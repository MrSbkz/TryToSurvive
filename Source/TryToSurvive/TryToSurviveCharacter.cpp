#include "TryToSurviveCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Models/Building/GateActor.h"

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

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
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
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATryToSurviveCharacter::OpenGates);
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
}

void ATryToSurviveCharacter::OnRotationStart()
{
	BuildingComponent->ProcessRotation();
}

void ATryToSurviveCharacter::OnRotationComplete()
{
	BuildingComponent->ProcessRotation();
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

void ATryToSurviveCharacter::OpenGates()
{
	FHitResult HitResult;
	FVector StartLocation(ForceInitToZero), EndLocation(ForceInitToZero);
	SetStartEndLocation(StartLocation, EndLocation);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_EngineTraceChannel1);

	auto res = HitResult.GetActor();
	
	if(AGateActor* Gate = Cast<AGateActor>(HitResult.GetActor()))
	{
		Gate->Interact();
	}
}

void ATryToSurviveCharacter::SetStartEndLocation(FVector& StartLocation, FVector& EndLocation) const
{
	StartLocation = GetActorLocation();
	EndLocation = StartLocation + FindComponentByClass<UCameraComponent>()->GetForwardVector() + 850.0f;
}
