#pragma once

#include "CoreMinimal.h"
#include "WidgetMenuBase.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "TryToSurvive/Models/Building/BuildingItemInfo.h"
#include "BuildingMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingItemChosen, TSubclassOf<ABuildingActorBase>&, BuildingItem);

UCLASS()
class UBuildingMenu : public UWidgetMenuBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FOnBuildingItemChosen OnBuildingItemChosen;
	
	void SetMenuButtons(TArray<FBuildingItemInfo>& BuildingItems);

	virtual void CloseMenu() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* ButtonsVerticalBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* BuildButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* ResourcesVerticalBox;

private:
	UFUNCTION()
	void SetSelectedItem(FBuildingItemInfo& BuildingItem);

	UFUNCTION()
	void BuildItem();

	UPROPERTY()
	FBuildingItemInfo SelectedItem;

	// Temp value, it'll need to check resources amount from inventory
	int CurrentResourceAmount = 3;
};
