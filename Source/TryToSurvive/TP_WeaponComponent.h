#pragma once
	
#include "Engine/World.h" 
#include "Enums/AttackState.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TP_WeaponComponent.generated.h"

class ATryToSurviveCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTP_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void Attack(AActor* DamagedActor, float BaseDamage, EAttackState AttackState);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	UAnimMontage* ChopAnimation;

private:
	UPROPERTY()
	ACharacter* Owner;
};
