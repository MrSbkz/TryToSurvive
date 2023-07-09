#pragma once
#include "TryToSurvive/Enums/BuildingMaterialType.h"
#include "TryToSurvive/Models/Building/BuildingActorBase.h"

#include "BuildingComponent.generated.h"

class ATryToSurviveCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTTS_BuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTTS_BuildingComponent();

	virtual void BeginPlay() override;

	void ResetBuilding();

	void Build();

	void SetPlayerController(APlayerController* PlayerController);

	void ProcessRotationMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Building Items")
	TArray<TSubclassOf<ABuildingActorBase>> BuildingItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace")
	float MaxBuildingDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace")
	float GridSnapValue = 25.0f;

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
	TArray<UMaterialInstanceDynamic*> CurrentMaterials;

	FVector BuildingSpawnLocation;

	bool IsRotating;

	bool IsBuildingEnable;

	FVector CurrentBuildingExtend;

	void StartPreview(const FHitResult& HitResult);

	void SetBuildingLocation(const FHitResult& HitResult);

	void CreateBuildingItem(EBuildingMaterialType MaterialType, const FHitResult& HitResult);

	void SetBuildingMaterial(EBuildingMaterialType MaterialType);

	void DrawTrace(FHitResult& HitResult, TArray<AActor*> IgnoredActors);

	void SetStartEndLocation(FVector& StartLocation, FVector& EndLocation);

	bool IsBuildingOnGround();

	void SetCurrentMaterials(TArray<UMaterialInterface*> Materials);
};
