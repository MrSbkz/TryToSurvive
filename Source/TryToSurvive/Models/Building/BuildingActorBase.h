#pragma once

#include "Components/BoxComponent.h"
#include "TryToSurvive/Models/ActorWithHealthBase.h"
#include "BuildingActorBase.generated.h"

UCLASS(ClassGroup=(Custom))
class ABuildingActorBase : public AActorWithHealthBase
{	
	GENERATED_BODY()

public:
	ABuildingActorBase();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* MeshComponent;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	// UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Materials")
	TArray<UMaterialInterface*> PreviewMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Materials")
	TArray<UMaterialInterface*> BaseMaterials;

	bool IsBuildingEnable = true;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
