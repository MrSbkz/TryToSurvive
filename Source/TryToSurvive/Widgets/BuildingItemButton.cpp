#include "BuildingItemButton.h"

#include "Components/TextBlock.h"

UBuildingItemButton::UBuildingItemButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBuildingItemButton::SetMainButton()
{
	MainButton = NewObject<UButton>(this);	
	UTextBlock* TextBlock = NewObject<UTextBlock>(this);
	if (TextBlock)
	{
		TextBlock->SetText(FText::FromString(BuildingItem.Name));
		MainButton->AddChild(TextBlock);
	}
	MainButton->SetToolTipText(FText::FromString(BuildingItem.Name));
	MainButton->OnClicked.AddUniqueDynamic(this, &UBuildingItemButton::OnButtonClicked);
}

void UBuildingItemButton::OnButtonClicked()
{
	OnClickedSignature.Broadcast(BuildingItem);
}
