#pragma once

#include "CoreMinimal.h"
#include "WallActor.h"
#include "Components/TimelineComponent.h"
#include "TryToSurvive/Models/Interfaces/InteractiveActor.h"
#include "GateActor.generated.h"

UCLASS()
class TRYTOSURVIVE_API AGateActor : public AWallActor, public IInteractiveActor
{
	GENERATED_BODY()
public:
	AGateActor();

	virtual void SetBuildingMaterials(const EBuildingMaterialType MaterialType) override;

	virtual void Interact_Implementation() override;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SetPreviewMaterialsColor() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBuildingStaticMeshComponent* LeftDoorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBuildingStaticMeshComponent* RightDoorComponent;

	FTimeline Timeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

private:
	void SetCollisions();

	UFUNCTION()
	void OpenDoors(float Value);
	
	float DoorsRotateAngle = 90.0f;

	bool IsGateOpened = false;
};
