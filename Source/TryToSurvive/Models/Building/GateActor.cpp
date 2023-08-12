#include "GateActor.h"

AGateActor::AGateActor()
{
	LeftDoorComponent = CreateDefaultSubobject<UBuildingStaticMeshComponent>("LeftDoorComponent");
	LeftDoorComponent->SetupAttachment(MeshComponent);
	RightDoorComponent = CreateDefaultSubobject<UBuildingStaticMeshComponent>("RightDoorComponent");
	RightDoorComponent->SetupAttachment(MeshComponent);
	
	LeftDoorComponent->BoxComponent = CreateDefaultSubobject<UBoxComponent>("LeftDoorCollision");
	LeftDoorComponent->BoxComponent->SetupAttachment(LeftDoorComponent);

	RightDoorComponent->BoxComponent = CreateDefaultSubobject<UBoxComponent>("RightDoorCollision");
	RightDoorComponent->BoxComponent->SetupAttachment(RightDoorComponent);
}

void AGateActor::SetBuildingMaterials(const EBuildingMaterialType MaterialType)
{
	Super::SetBuildingMaterials(MaterialType);

	LeftDoorComponent->SetMaterials(MaterialType);
	RightDoorComponent->SetMaterials(MaterialType);
}

void AGateActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector LeftDoorExtent = LeftDoorComponent->GetStaticMesh()->GetBoundingBox().GetExtent();
	LeftDoorComponent->BoxComponent->SetBoxExtent(LeftDoorExtent);
	LeftDoorComponent->BoxComponent->SetRelativeLocation(FVector(0.0f, LeftDoorExtent.Y, LeftDoorExtent.Z));
	LeftDoorComponent->BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftDoorComponent->BoxComponent->SetCollisionObjectType(ECC_WorldStatic);

	const FVector RightDoorExtent = LeftDoorComponent->GetStaticMesh()->GetBoundingBox().GetExtent();
	RightDoorComponent->BoxComponent->SetBoxExtent(RightDoorExtent);
	RightDoorComponent->BoxComponent->SetRelativeLocation(FVector(0.0f, -LeftDoorExtent.Y, RightDoorExtent.Z));
	RightDoorComponent->BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightDoorComponent->BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
}

void AGateActor::SetPreviewMaterialsColor()
{
	Super::SetPreviewMaterialsColor();

	LeftDoorComponent->SetPreviewMaterialsColor(CurrentPreviewColor);
	RightDoorComponent->SetPreviewMaterialsColor(CurrentPreviewColor);
}
