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

void AGateActor::Interact_Implementation()
{
	if(IsGateOpened)
	{
		Timeline.Reverse();
	}
	else
	{
		Timeline.Play();
	}

	IsGateOpened = !IsGateOpened;
	UE_LOG(LogTemp, Error, TEXT("[%S] Gate is %S"), __FUNCTION__, IsGateOpened ? "opened" : "closed");
}

void AGateActor::BeginPlay()
{
	Super::BeginPlay();

	SetCollisions();

	if(CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AGateActor::OpenDoors);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
	
}

void AGateActor::SetPreviewMaterialsColor()
{
	Super::SetPreviewMaterialsColor();

	LeftDoorComponent->SetPreviewMaterialsColor(CurrentPreviewColor);
	RightDoorComponent->SetPreviewMaterialsColor(CurrentPreviewColor);
}

void AGateActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Timeline.TickTimeline(DeltaSeconds);
}

void AGateActor::SetCollisions()
{
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

void AGateActor::OpenDoors(float Value)
{
	const FRotator LeftDoorRotator = FRotator(0.0f, DoorsRotateAngle * Value, 0.0f);
	const FRotator RightDoorRotator = FRotator(0.0f, DoorsRotateAngle * -Value, 0.0f);
	LeftDoorComponent->SetRelativeRotation(LeftDoorRotator);
	RightDoorComponent->SetRelativeRotation(RightDoorRotator);
}
