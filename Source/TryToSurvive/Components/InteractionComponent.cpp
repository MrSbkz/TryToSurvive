#include "InteractionComponent.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TryToSurvive/Models/Building/GateActor.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	if (APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InteractionMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent =
			Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &UInteractionComponent::Interact);
		}
	}
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TArray<AActor*> IgnoredActors{Owner};

	DrawTrace(HitResult, IgnoredActors);
	if (AActor* HitActor = Cast<AActor>(HitResult.GetActor()))
	{
		CurrentInteractiveActor = HitActor;
	}
	else
	{
		CurrentInteractiveActor = nullptr;
	}
}


void UInteractionComponent::DrawTrace(FHitResult& HitResult, const TArray<AActor*> IgnoredActors) const
{
	FVector StartLocation(ForceInitToZero), EndLocation(ForceInitToZero);
	SetStartEndLocation(StartLocation, EndLocation);

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.5f);
}

void UInteractionComponent::SetStartEndLocation(FVector& StartLocation, FVector& EndLocation) const
{
	StartLocation = Owner->GetActorLocation();
	EndLocation = StartLocation + Owner->FindComponentByClass<UCameraComponent>()->GetForwardVector() * MaxInteractiveDistance;
}

void UInteractionComponent::Interact()
{
	if (IInteractiveActor* InteractiveActor = Cast<IInteractiveActor>(CurrentInteractiveActor))
	{
		InteractiveActor->Execute_Interact(CurrentInteractiveActor);
	}
}
