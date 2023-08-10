#include "BaseWoodActor.h"

ABaseWoodActor::ABaseWoodActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
}
