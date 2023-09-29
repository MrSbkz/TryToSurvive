#include "AxeActor.h"

AAxeActor::AAxeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CollisionComponent");
	SetRootComponent(Mesh);
	CapsuleComponent->SetupAttachment(Mesh);
}

void AAxeActor::BeginPlay()
{
	Super::BeginPlay();
}