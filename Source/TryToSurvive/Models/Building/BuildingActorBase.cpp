#include "BuildingActorBase.h"

#include "Kismet/KismetSystemLibrary.h"

ABuildingActorBase::ABuildingActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	BoxComponent->SetupAttachment(MeshComponent);
}

bool ABuildingActorBase::SetLocation(FVector& BuildingLocation)
{
	return true;
}


