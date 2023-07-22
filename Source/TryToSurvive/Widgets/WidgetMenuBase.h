#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMenuBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetIgnorePlayerMovement, bool, IsEnabled);

UCLASS()
class UWidgetMenuBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FSetIgnorePlayerMovement SetIgnorePlayerMovement;

	void OpenMenu();

	virtual void CloseMenu();
};
