#include "StumpActor.h"

AStumpActor::AStumpActor()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	BoxCollision->SetupAttachment(MeshComponent);
}

void AStumpActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector Extent = MeshComponent->GetStaticMesh()->GetBoundingBox().GetExtent();

	BoxCollision->SetBoxExtent(FVector(Extent.X, Extent.Y, Extent.Y));
	BoxCollision->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Rotation().Yaw));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionObjectType(ECC_WorldStatic);
}


