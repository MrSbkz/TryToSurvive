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
	if(CurrentBuildItem)
	{
		CurrentBuildItem->SetActorLocation(BuildingSpawnLocation);
	}
	else
	{
		CreateBuildingItem(BuildingMaterialPairs[0].Preview);
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

	CurrentBuildItem->SetActorEnableCollision(true);
	CreateBuildingItem(BuildingMaterialPairs[0].Base);

	CurrentBuildItem = nullptr;
	CurrentMaterial = nullptr;
}

void UTTS_BuildingComponent::SetPlayerController(APlayerController* PlayerController)
{
	CharacterPlayerController = PlayerController;
}

void UTTS_BuildingComponent::StartRotation()
{
	if(IsBuildingMode)
	{
		IsRotating = true;
	}
}

void UTTS_BuildingComponent::CompleteRotation()
{
	if(IsBuildingMode)
	{
		IsRotating = false;
	}
}

void UTTS_BuildingComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CharacterPlayerController && Owner)
	{
		if(IsBuildingMode)
		{
			const FRotator SpawnRotation = CharacterPlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector PlayerLocation = Owner->GetActorLocation();

			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			BuildingSpawnLocation = PlayerLocation + SpawnRotation.RotateVector(BuildItemOffset);

			// TODO: Replace this, z location won't be always 0
			BuildingSpawnLocation.Z = 0.0f;

			StartPreview();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] PlayerController or Owner is nullptr"), __FUNCTION__);
	}

	if(IsRotating)
	{
		BuildingRotation = CurrentBuildItem->GetActorRotation();
		BuildingRotation.Yaw++;
		CurrentBuildItem->SetActorRotation(BuildingRotation);
	}
}

void UTTS_BuildingComponent::CreateBuildingItem(UMaterialInterface* Material)
{
	const FTransform Transform;

	if(!CurrentBuildItem)
	{
		CurrentBuildItem = GetWorld()->SpawnActor<ABuildingActorBase>(BuildingMaterialPairs[0].BuildingItem, Transform);
	}

	if (CurrentBuildItem && GetWorld() && Material)
	{
		CurrentMaterial = UMaterialInstanceDynamic::Create(Material, GetWorld());
		CurrentBuildItem->MeshComponent->SetMaterial(0, CurrentMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] Material has not been set"), __FUNCTION__);
	}
}
