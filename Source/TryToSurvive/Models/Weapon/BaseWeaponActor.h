#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponActor.generated.h"

UCLASS()
class TRYTOSURVIVE_API ABaseWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeaponActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	USkeletalMeshComponent* Mesh;
};
