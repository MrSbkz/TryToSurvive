#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/CapsuleComponent.h>
#include <TryToSurvive/Models/Weapon/BaseWeaponActor.h>
#include "AxeActor.generated.h"

UCLASS()
class TRYTOSURVIVE_API AAxeActor : public ABaseWeaponActor
{
	GENERATED_BODY()

public:
	AAxeActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Collision)
	UCapsuleComponent* CapsuleComponent;

protected:
	virtual void BeginPlay() override;
};
