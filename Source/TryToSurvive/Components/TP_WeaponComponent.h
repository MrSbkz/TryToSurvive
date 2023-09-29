#pragma once
	
#include "Engine/World.h" 
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include <TryToSurvive/Models/Trees/ChoppableTree.h>
#include <TryToSurvive/Models/Weapon/BaseWeaponActor.h>
#include <TryToSurvive/Models/Weapon/Hatchet/HatchetActor.h>
#include "TP_WeaponComponent.generated.h"

class ATryToSurviveCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTP_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void Attack(float BaseDamage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	UAnimMontage* ChopAnimation;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<class ABaseWeaponActor> AxeActor;

	void AttachWeapon(USceneComponent* WeaponSocket, FName SocketName);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace")
	float MaxInteractiveDistance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	// ReSharper disable once UnrealHeaderToolParserError
	UInputMappingContext* WeaponMappingContext;

private:
	AHatchetActor* WeaponInHand;
	
	UPROPERTY()
	ACharacter* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY()
	AActor* CurrentInteractiveActor;

	void DrawTrace(FHitResult& HitResult, TArray<AActor*> IgnoredActors) const;

	void SetStartEndLocation(FVector& StartLocation, FVector& EndLocation) const;
};
