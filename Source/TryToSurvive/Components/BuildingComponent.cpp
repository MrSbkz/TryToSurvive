#include "BuildingComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TryToSurvive/TryToSurviveCharacter.h"

UBuildingComponent::UBuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ATryToSurviveCharacter>(GetOwner());
	CurrentPreviewColor = FLinearColor::White;
}

void UBuildingComponent::SetBuildingMode()
{
	IsBuildingMode = !IsBuildingMode;
}

void UBuildingComponent::ResetBuilding()
{
	CurrentBuildItem->Destroy();
	CurrentBuildItem = nullptr;
}

void UBuildingComponent::Build()
{
	if (!IsBuildingEnable) return;

	IsBuildingMode = false;

	CurrentBuildItem->BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SetBuildingMaterial(EBuildingMaterialType::Base);

	CurrentBuildItem = nullptr;
	CurrentMaterials.Empty();
}

void UBuildingComponent::ProcessRotation() const
{
	if (IsBuildingMode)
	{
		FRotator BuildingRotation = CurrentBuildItem->GetActorRotation();
		BuildingRotation.Yaw += 30.0f;
		CurrentBuildItem->SetActorRotation(BuildingRotation);
	}
}

void UBuildingComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Owner)
	{
		if (IsBuildingMode)
		{
			FHitResult HitResult;
			TArray<AActor*> IgnoredActors{Owner};
			if (CurrentBuildItem)
			{
				IgnoredActors.Add(CurrentBuildItem);
			}

			DrawTrace(HitResult, IgnoredActors);
			StartPreview(HitResult);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] Owner is nullptr"), __FUNCTION__);
	}
}

void UBuildingComponent::StartPreview(const FHitResult& HitResult)
{
	if (CurrentBuildItem)
	{
		SetBuildingLocation(HitResult);
	}
	else
	{
		CreateBuildingItem(EBuildingMaterialType::Preview, HitResult);
	}
}

void UBuildingComponent::SetBuildingLocation(const FHitResult& HitResult)
{
	FVector BuildingLocation;
	if (HitResult.bBlockingHit)
	{
		BuildingLocation = HitResult.Location.GridSnap(GridSnapValue);
	}
	else
	{
		FVector StartLocation(ForceInitToZero), EndLocation(ForceInitToZero);
		SetStartEndLocation(StartLocation, EndLocation);

		BuildingLocation = EndLocation;
	}

	IsBuildingEnable = CurrentBuildItem->SetLocation(BuildingLocation);

	if (IsBuildingEnable && CurrentPreviewColor != FLinearColor::Green)
	{
		SetPreviewMaterialsColor(FLinearColor::Green);
	}
	else if (CurrentPreviewColor != FLinearColor::Red)
	{
		SetPreviewMaterialsColor(FLinearColor::Red);
	}
}

void UBuildingComponent::CreateBuildingItem(const EBuildingMaterialType MaterialType, const FHitResult& HitResult)
{
	FTransform Transform;
	Transform.SetLocation(HitResult.Location);
	CurrentBuildItem = GetWorld()->SpawnActor<ABuildingActorBase>(BuildingItems[0], Transform);

	SetBuildingMaterial(MaterialType);
}

void UBuildingComponent::SetBuildingMaterial(const EBuildingMaterialType MaterialType)
{
	if (CurrentBuildItem &&
		GetWorld() &&
		CurrentBuildItem->PreviewMaterials.Num() &&
		CurrentBuildItem->BaseMaterials.Num())
	{
		CurrentMaterials.Empty();
		switch (MaterialType)
		{
		case EBuildingMaterialType::Preview:
			SetCurrentMaterials(CurrentBuildItem->PreviewMaterials);
			break;
		case EBuildingMaterialType::Base:
			SetCurrentMaterials(CurrentBuildItem->BaseMaterials);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("[%S] Material type has not been set"), __FUNCTION__);
			break;
		}

		for (int i = 0; i < CurrentMaterials.Num(); i++)
		{
			CurrentBuildItem->MeshComponent->SetMaterial(i, CurrentMaterials[i]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%S] Material has not been set"), __FUNCTION__);
	}
}

void UBuildingComponent::DrawTrace(FHitResult& HitResult, const TArray<AActor*> IgnoredActors) const
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

void UBuildingComponent::SetStartEndLocation(FVector& StartLocation, FVector& EndLocation) const
{
	StartLocation = Owner->GetActorLocation();
	EndLocation = StartLocation + Owner->FindComponentByClass<UCameraComponent>()->GetForwardVector() *
		MaxBuildingDistance;
}

void UBuildingComponent::SetCurrentMaterials(TArray<UMaterialInterface*> Materials)
{
	for (const auto Material : Materials)
	{
		CurrentMaterials.Add(UMaterialInstanceDynamic::Create(Material, GetWorld()));
	}
}

void UBuildingComponent::SetPreviewMaterialsColor(const FLinearColor Color)
{
	for (const auto Material : CurrentMaterials)
	{
		Material->SetVectorParameterValue(FName("Preview Color"), Color);
	}
}
