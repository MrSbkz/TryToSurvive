#include "StumpActor.h"

AStumpActor::AStumpActor()
{
	SetRootComponent(MeshComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	BoxCollision->SetupAttachment(MeshComponent);
}

void AStumpActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector Extent = MeshComponent->GetStaticMesh()->GetBoundingBox().GetExtent();

	BoxCollision->SetBoxExtent(FVector(Extent.X, Extent.Y, Extent.Z));
	BoxCollision->SetRelativeLocation(FVector(0.0f, 0.0f, Extent.Z));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionObjectType(ECC_WorldStatic);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Block);
}

