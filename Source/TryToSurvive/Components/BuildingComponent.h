#pragma once
#include "TryToSurvive/Models/Building/BuildingActorBase.h"
#include "TryToSurvive/Models/Building/FBuildingMaterialPair.h"

#include "BuildingComponent.generated.h"

class ATryToSurviveCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTTS_BuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTTS_BuildingComponent();

	virtual void BeginPlay() override;

	void StartPreview();

	void ResetBuilding();

	void Build();

	void SetPlayerController(APlayerController* PlayerController);

	void StartRotation();

	void CompleteRotation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Materials")
	TArray<FBuildingMaterialPair> BuildingMaterialPairs;

	bool IsBuildingMode = false;

protected:
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Build")
	FVector BuildItemOffset;

private:
	UPROPERTY()
	ABuildingActorBase* CurrentBuildItem;

	UPROPERTY()
	APlayerController* CharacterPlayerController;

	UPROPERTY()
	ATryToSurviveCharacter* Owner;

	UPROPERTY()
	UMaterialInstanceDynamic* CurrentMaterial;

	FVector BuildingSpawnLocation;

	FRotator BuildingRotation;

	bool IsRotating;

	void CreateBuildingItem(UMaterialInterface* Material);
};
