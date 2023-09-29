#include "HatchetActor.h"

AHatchetActor::AHatchetActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CollisionComponent");
	SetRootComponent(Mesh);
	CapsuleComponent->SetupAttachment(Mesh);
}

void AHatchetActor::BeginPlay()
{
	Super::BeginPlay();
}

