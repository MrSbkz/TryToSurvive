#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWoodActor.generated.h"

UCLASS()
class TRYTOSURVIVE_API ABaseWoodActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWoodActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
		UStaticMeshComponent* MeshComponent;
};
