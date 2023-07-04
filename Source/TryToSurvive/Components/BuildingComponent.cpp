#include "BuildingComponent.h"

#include "TryToSurvive/TryToSurviveCharacter.h"

UTTS_BuildingComponent::UTTS_BuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTTS_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ATryToSurviveCharacter>(GetOwner());
}

void UTTS_BuildingComponent::StartPreview()
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
		CurrentBuildItem = GetWorld()->SpawnActor<ABuildingActorBase>(BuildingActors[0], Transform);
		CurrentBuildItem->SetActorEnableCollision(false);
	}
}

void UTTS_BuildingComponent::ResetBuilding()
{
	CurrentBuildItem->Destroy();
	CurrentBuildItem = nullptr;
}

void UTTS_BuildingComponent::Build()
{
	IsBuildingMode = false;
	FTransform Transform;
	Transform.SetLocation(BuildingSpawnLocation);
	GetWorld()->SpawnActor<ABuildingActorBase>(BuildingActors[0], Transform);
}

void UTTS_BuildingComponent::SetPlayerController(APlayerController* PlayerController)
{
	CharacterPlayerController = PlayerController;
}

void UTTS_BuildingComponent::TickComponent(
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