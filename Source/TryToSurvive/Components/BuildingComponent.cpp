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
		CreateBuildingItem(EBuildingMaterialType::Preview);
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
	CreateBuildingItem(EBuildingMaterialType::Base);

	CurrentBuildItem = nullptr;
	CurrentMaterial = nullptr;
}

void UTTS_BuildingComponent::SetPlayerController(APlayerController* PlayerController)
{
	CharacterPlayerController = PlayerController;
}

void UTTS_BuildingComponent::ProcessRotationMode()
{
	if(IsBuildingMode)
	{
		IsRotating = !IsRotating;
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
		FRotator BuildingRotation = CurrentBuildItem->GetActorRotation();
		BuildingRotation.Yaw++;
		CurrentBuildItem->SetActorRotation(BuildingRotation);
	}
}

void UTTS_BuildingComponent::CreateBuildingItem(EBuildingMaterialType MaterialType)
{
	if(!CurrentBuildItem)
	{
		const FTransform Transform;
		CurrentBuildItem = GetWorld()->SpawnActor<ABuildingActorBase>(BuildingItems[0], Transform);
	}
	
	if (CurrentBuildItem && GetWorld() && CurrentBuildItem->PreviewMaterial && CurrentBuildItem->BaseMaterial)
	{
		switch (MaterialType)
		{
		case EBuildingMaterialType::Preview:
			CurrentMaterial = UMaterialInstanceDynamic::Create(CurrentBuildItem->PreviewMaterial, GetWorld());
			break;
		case EBuildingMaterialType::Base:
			CurrentMaterial = UMaterialInstanceDynamic::Create(CurrentBuildItem->BaseMaterial, GetWorld());
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("[%S] Material type has not been set"), __FUNCTION__);
			break;
		}

		CurrentBuildItem->MeshComponent->SetMaterial(0, CurrentMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] Material has not been set"), __FUNCTION__);
	}
}
