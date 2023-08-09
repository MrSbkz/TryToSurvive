#include "BuildingStaticMeshComponent.h"

void UBuildingStaticMeshComponent::SetMaterials(EBuildingMaterialType MaterialType)
{
	CurrentMaterials.Empty();
	switch (MaterialType)
	{
	case EBuildingMaterialType::Preview:
		SetCurrentMaterials(PreviewMaterials);
		break;
	case EBuildingMaterialType::Base:
		BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
		SetCurrentMaterials(BaseMaterials);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("[%S] Material type has not been set"), __FUNCTION__);
		break;
	}

	for (int i = 0; i < CurrentMaterials.Num(); i++)
	{
		SetMaterial(i, CurrentMaterials[i]);
	}
}

void UBuildingStaticMeshComponent::SetupCollisions(FName CollisionName)
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(CollisionName);
	BoxComponent->SetupAttachment(this);
}

void UBuildingStaticMeshComponent::SetPreviewMaterialsColor(FLinearColor Color)
{
	for (const auto Material : CurrentMaterials)
	{
		Material->SetVectorParameterValue(FName("Preview Color"), Color);
	}
}

void UBuildingStaticMeshComponent::SetCurrentMaterials(TArray<UMaterialInterface*> Materials)
{
	for (const auto Material : Materials)
	{
		CurrentMaterials.Add(UMaterialInstanceDynamic::Create(Material, GetWorld()));
	}
}
