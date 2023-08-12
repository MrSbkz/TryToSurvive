#pragma once

#include "TryToSurvive/Models/Building/BuildingActorBase.h"
#include "TryToSurvive/Models/Building/BuildingItemInfo.h"
#include "TryToSurvive/Widgets/BuildingMenu.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

#include "BuildingComponent.generated.h"

class ATryToSurviveCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuildingComponent();

	virtual void BeginPlay() override;

	void SetBuildingMode();

	void CreateBuildingMenu();

	void ResetBuilding();

	void Build();

	void ProcessRotation() const;

	bool IsBuildingMode = false;

	UPROPERTY()
	UBuildingMenu* BuildingMenu;

protected:
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Building Menu")
	TSubclassOf<UBuildingMenu> BuildingMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Building Items")
	TArray<FBuildingItemInfo> BuildingItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace")
	float MaxBuildingDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Trace")
	float GridSnapValue = 25.0f;

private:
	void StartPreview(const FHitResult& HitResult);

	void SetBuildingLocation(const FHitResult& HitResult);

	void CreateBuildingItem(const FHitResult& HitResult);

	void DrawTrace(FHitResult& HitResult, TArray<AActor*> IgnoredActors) const;

	void SetStartEndLocation(FVector& StartLocation, FVector& EndLocation) const;

	UFUNCTION()
	void SetCurrentBuildingItem(TSubclassOf<ABuildingActorBase>& BuildingItem);

	UPROPERTY()
	TSubclassOf<ABuildingActorBase> CurrentBuildItemClass;

	UPROPERTY()
	ABuildingActorBase* CurrentBuildingItem;

	UPROPERTY()
	ACharacter* Owner;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CurrentMaterials;

	FVector BuildingSpawnLocation;

	bool IsBuildingEnable;

	bool IsBuildingMenuOpened = false;

	FVector CurrentBuildingExtend;

	FLinearColor CurrentPreviewColor;
};
