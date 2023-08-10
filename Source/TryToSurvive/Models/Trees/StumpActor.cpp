#include "StumpActor.h"

AStumpActor::AStumpActor()
{
	SetRootComponent(MeshComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	BoxComponent->SetupAttachment(MeshComponent);

	BoxComponent->SetBoxExtent(FVector(22.0f, 22.0f, 22.0f));
	BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 22.0f));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
}


