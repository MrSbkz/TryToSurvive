#include "BaseWeaponActor.h"

ABaseWeaponActor::ABaseWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
}

