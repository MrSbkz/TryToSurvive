#pragma once

#include "BaseWoodActor.h"
#include "Components/CapsuleComponent.h"
#include "LogActor.generated.h"

UCLASS(ClassGroup=(Custom))
class ALogActor : public ABaseWoodActor
{
	GENERATED_BODY()

public:
	ALogActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Collision")
	UCapsuleComponent* CapsuleComponent;

protected:
	virtual void BeginPlay() override;
};
