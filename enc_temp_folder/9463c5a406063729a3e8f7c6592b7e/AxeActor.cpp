#include "AxeActor.h"

AAxeActor::AAxeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CollisionComponent");
	SetRootComponent(Mesh);
	CapsuleComponent->SetupAttachment(Mesh);

	Mesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void AAxeActor::BeginPlay()
{
	Super::BeginPlay();
}