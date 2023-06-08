#include "BuildingComponent.h"

#include "TryToSurvive/TryToSurviveCharacter.h"

UBC_BuildingComponent::UBC_BuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBC_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ATryToSurviveCharacter>(GetOwner());
}

void UBC_BuildingComponent::StartPreview()
{
	if(!IsBuildingMode) return;
	
	if(CurrentBuildItem)
	{
		CurrentBuildItem->SetActorLocation(BuildingSpawnLocation);
	}
	else
	{
		FTransform Transform;
		Transform.SetLocation(BuildingSpawnLocation);
		CurrentBuildItem = GetWorld()->SpawnActor<AActor>(BlockClasses[0], Transform);
		CurrentBuildItem->SetActorEnableCollision(false);
	}
}

void UBC_BuildingComponent::ResetBuilding()
{
	CurrentBuildItem->Destroy();
	CurrentBuildItem = nullptr;
}

void UBC_BuildingComponent::Build()
{
	IsBuildingMode = false;
	FTransform Transform;
	Transform.SetLocation(BuildingSpawnLocation);
	AActor* Item = GetWorld()->SpawnActor<AActor>(BlockClasses[0], Transform);
}

void UBC_BuildingComponent::SetPlayerController(APlayerController* PlayerController)
{
	CharacterPlayerController = PlayerController;
}

void UBC_BuildingComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CharacterPlayerController && Owner)
	{
		const FRotator SpawnRotation = CharacterPlayerController->PlayerCameraManager->GetCameraRotation();
		const FVector PlayerLocation = Owner->GetActorLocation();

		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		BuildingSpawnLocation = PlayerLocation + SpawnRotation.RotateVector(BuildItemOffset);
		BuildingSpawnLocation.Z = 0.0f;
		StartPreview();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] PlayerController or Owner is nullptr"), __FUNCTION__);
	}
}