#pragma once

#include "Components/Button.h"
#include "TryToSurvive/Models/Building/BuildingItemInfo.h"
#include "BuildingItemButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedSignature, FBuildingItemInfo&, Button);

UCLASS()
class UBuildingItemButton : public UUserWidget
{
	GENERATED_BODY()
public:	
	void SetMainButton();

	UPROPERTY()
	FOnClickedSignature OnClickedSignature;
	
	UPROPERTY(meta=(BindWidget))
	UButton* MainButton;
	
	UPROPERTY()
	FBuildingItemInfo BuildingItem;

private:
	UFUNCTION()
	void OnButtonClicked();
};
