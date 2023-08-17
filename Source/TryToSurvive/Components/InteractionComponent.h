#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRYTOSURVIVE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	// ReSharper disable once UnrealHeaderToolParserError
	UInputMappingContext* InteractionMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace")
	float MaxInteractiveDistance = 100.0f;

private:
	void DrawTrace(FHitResult& HitResult, TArray<AActor*> IgnoredActors) const;

	void SetStartEndLocation(FVector& StartLocation, FVector& EndLocation) const;

	void Interact();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY()
	ACharacter* Owner;

	UPROPERTY()
	AActor* CurrentInteractiveActor;
};
