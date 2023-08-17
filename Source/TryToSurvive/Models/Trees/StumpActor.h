#pragma once

#include "Components/BoxComponent.h"
#include "BaseWoodActor.h"
#include "StumpActor.generated.h"

UCLASS(ClassGroup=(Custom))
class AStumpActor : public ABaseWoodActor
{
	GENERATED_BODY()
	
public:
	AStumpActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Collision")
	UBoxComponent* BoxCollision;

protected:
	virtual void BeginPlay() override;
};
