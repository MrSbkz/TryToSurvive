#include "LogActor.h"

ALogActor::ALogActor()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CollisionComponent");
	CapsuleComponent->SetupAttachment(MeshComponent);

	CapsuleComponent->SetCapsuleSize(12.0f, 115.0f);
	CapsuleComponent->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
}

