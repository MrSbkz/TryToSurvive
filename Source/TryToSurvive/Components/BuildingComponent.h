#pragma once

#include "BuildingComponent.generated.h"

class ATryToSurviveCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UBC_BuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBC_BuildingComponent();

	virtual void BeginPlay() override;

	void StartPreview();

	void Build();

	void SetPlayerController(APlayerController* PlayerController);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Build")
	TArray<TSubclassOf<AActor>> BlockClasses;

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
	AActor* CurrentBuildItem;

	UPROPERTY()
	APlayerController* CharacterPlayerController;

	UPROPERTY()
	ATryToSurviveCharacter* Owner;

	FVector BuildingSpawnLocation;
};
