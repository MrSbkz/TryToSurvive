#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/CapsuleComponent.h>
#include <TryToSurvive/Models/Weapon/BaseWeaponActor.h>
#include "HatchetActor.generated.h"

UCLASS()
class TRYTOSURVIVE_API AHatchetActor : public ABaseWeaponActor
{
	GENERATED_BODY()
	
public:
	AHatchetActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Collision)
	UCapsuleComponent* CapsuleComponent;

protected:
	virtual void BeginPlay() override;
};
