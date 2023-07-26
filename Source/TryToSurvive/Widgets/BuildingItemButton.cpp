#include "BuildingItemButton.h"

#include "Components/TextBlock.h"

void UBuildingItemButton::SetMainButton()
{
	MainButton = NewObject<UButton>(this);
	if (UTextBlock* TextBlock = NewObject<UTextBlock>(this))
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
