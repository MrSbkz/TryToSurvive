#include "BuildingComponent.h"
#include "Camera/CameraComponent.h"
#include "TryToSurvive/Enums/BuildingMaterialType.h"

UBuildingComponent::UBuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	CurrentPreviewColor = FLinearColor::White;
}

void UBuildingComponent::SetBuildingMode()
{
	IsBuildingMenuOpened = !IsBuildingMenuOpened;
	if (BuildingMenu && !BuildingMenu->IsInViewport())
	{
		BuildingMenu->AddToViewport();
	}

	if (IsBuildingMenuOpened)
	{
		ResetBuilding();
		BuildingMenu->OpenMenu();
		BuildingMenu->SetMenuButtons(BuildingItems);
	}
	else
	{
		ResetBuilding();
		BuildingMenu->CloseMenu();
	}
}

void UBuildingComponent::CreateBuildingMenu()
{
	BuildingMenu = CreateWidget<UBuildingMenu>(GetWorld(), BuildingMenuClass);
	BuildingMenu->OnBuildingItemChosen.AddDynamic(this, &UBuildingComponent::SetCurrentBuildingItem);
}

void UBuildingComponent::ResetBuilding()
{
	if(CurrentBuildingItem)
	{		
		CurrentMaterials.Empty();
		CurrentBuildingItem->Destroy();
		CurrentBuildingItem = nullptr;
		IsBuildingMode = false;
	}
}

void UBuildingComponent::Build()
{
	if (!IsBuildingEnable) return;

	IsBuildingMode = false;

	CurrentBuildingItem->SetBuildingMaterials(EBuildingMaterialType::Base);

	CurrentBuildingItem = nullptr;
	CurrentMaterials.Empty();
}

void UBuildingComponent::ProcessRotation() const
{
	if (IsBuildingMode)
	{
		FRotator BuildingRotation = CurrentBuildingItem->GetActorRotation();
		BuildingRotation.Yaw += 30.0f;
		CurrentBuildingItem->SetActorRotation(BuildingRotation);
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
			if (CurrentBuildingItem)
			{
				IgnoredActors.Add(CurrentBuildingItem);
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
	if (CurrentBuildingItem)
	{
		SetBuildingLocation(HitResult);
	}
	else
	{
		CreateBuildingItem(HitResult);
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

	IsBuildingEnable = CurrentBuildingItem->SetLocation(BuildingLocation);
}

void UBuildingComponent::CreateBuildingItem(const FHitResult& HitResult)
{
	FTransform Transform;
	Transform.SetLocation(HitResult.Location);
	CurrentBuildingItem = GetWorld()->SpawnActor<ABuildingActorBase>(CurrentBuildItemClass, Transform);
	
	CurrentBuildingItem->SetBuildingMaterials(EBuildingMaterialType::Preview);
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

void UBuildingComponent::SetCurrentBuildingItem(TSubclassOf<ABuildingActorBase>& BuildingItem)
{
	CurrentBuildItemClass = BuildingItem;
	IsBuildingMode = true;
	IsBuildingMenuOpened = false;
	BuildingMenu->CloseMenu();
}
