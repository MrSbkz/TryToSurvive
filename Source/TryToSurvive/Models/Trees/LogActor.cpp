#include "LogActor.h"

ALogActor::ALogActor()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CollisionComponent");
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetupAttachment(MeshComponent);
	MeshComponent->SetupAttachment(CapsuleComponent);
}

void ALogActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector Extent = MeshComponent->GetStaticMesh()->GetBoundingBox().GetExtent();

	CapsuleComponent->SetCapsuleSize(Extent.X, Extent.Y);
	CapsuleComponent->SetWorldRotation(FRotator(0.0f, 0.0f, Extent.Rotation().Yaw));
	
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

