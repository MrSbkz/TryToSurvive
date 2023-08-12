#include "BuildingActorBase.h"

#include "Kismet/KismetSystemLibrary.h"

ABuildingActorBase::ABuildingActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UBuildingStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	MeshComponent->BoxComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	MeshComponent->BoxComponent->SetupAttachment(MeshComponent);
}

bool ABuildingActorBase::SetLocation(FVector& BuildingLocation)
{
	DefineLocation(BuildingLocation);
	IsBuildingEnable = CanBuild();
	
	if (IsBuildingEnable && CurrentPreviewColor != FLinearColor::Green)
	{
		CurrentPreviewColor = FLinearColor::Green;
		SetPreviewMaterialsColor();
	}
	else if (!IsBuildingEnable && CurrentPreviewColor != FLinearColor::Red)
	{
		CurrentPreviewColor = FLinearColor::Red;
		SetPreviewMaterialsColor();
	}
	
	return IsBuildingEnable;
}

void ABuildingActorBase::SetBuildingMaterials(const EBuildingMaterialType MaterialType)
{
	MeshComponent->SetMaterials(MaterialType);
}

void ABuildingActorBase::SetPreviewMaterialsColor()
{
	MeshComponent->SetPreviewMaterialsColor(CurrentPreviewColor);
}

void ABuildingActorBase::DefineLocation(FVector& BuildingLocation)
{
	FVector Origin(ForceInitToZero), Extend(ForceInitToZero);
	GetActorBounds(false, Origin, Extend);
	const FVector CurrentBuildingExtend = Extend;

	const TArray<AActor*> IgnoredActors = {this};
	TArray<FHitResult> BoxHits;
	FVector BoxLocation = BuildingLocation + CurrentBuildingExtend;
	BoxLocation.Y = BoxLocation.Y - CurrentBuildingExtend.Y;
	BoxLocation.X = BoxLocation.X - CurrentBuildingExtend.X;

	UKismetSystemLibrary::BoxTraceMulti(
		GetWorld(),
		BoxLocation,
		BoxLocation,
		CurrentBuildingExtend,
		GetActorRotation(),
		TraceTypeQuery8,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		BoxHits,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.5f
	);

	for (const auto& Hit : BoxHits)
	{
		BuildingLocation += Hit.Normal;
	}

	SetActorLocation(BuildingLocation);
}

