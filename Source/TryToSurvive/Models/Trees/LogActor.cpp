#include "LogActor.h"

ALogActor::ALogActor()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CollisionComponent");
	CapsuleComponent->SetupAttachment(MeshComponent);
}

void ALogActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector Extent = MeshComponent->GetStaticMesh()->GetBoundingBox().GetExtent();

	CapsuleComponent->SetCapsuleSize(Extent.X/2, Extent.Z/2);
	CapsuleComponent->SetWorldRotation(FRotator(0.0f, 0.0f, Extent.Rotation().Yaw));
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
}

