#pragma once

#include "CoreMinimal.h"
#include "Enums/AttackState.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include <Kismet/GameplayStatics.h>
#include "Components/CapsuleComponent.h"
#include "Components/BuildingComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "TryToSurviveCharacter.generated.h"

class UCameraComponent;

UCLASS(config=Game)
class ATryToSurviveCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	ATryToSurviveCharacter();

	EAttackState AttackState = EAttackState::Attack;
	TArray<AActor*> HarvestActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Components)
	UBuildingComponent* BuildingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimMontage* ChopAnimation;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void OnSwitchBuildMode();

	virtual void OnHit();

	void OnRotationStart();
	
	void OnRotationComplete();

	void OnToggleCamera();

	bool isEnabledAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* ThirdPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

private:
	void Chop();

	UFUNCTION()
	void SetIgnorePlayerMovement(bool IsEnabled);

	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ToggleCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	// ReSharper disable once UnrealHeaderToolParserError
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* BuildModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* HitAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* RotateAction;
};

