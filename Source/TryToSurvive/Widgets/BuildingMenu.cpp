#include "BuildingMenu.h"

#include "BuildingItemButton.h"
#include "Components/TextBlock.h"

void UBuildingMenu::SetMenuButtons(TArray<FBuildingItemInfo>& BuildingItems)
{
	for (const auto Item : BuildingItems)
	{
		UBuildingItemButton* NewButton = CreateWidget<UBuildingItemButton>(GetWorld(), UBuildingItemButton::StaticClass());
		NewButton->BuildingItem = Item;
		if (NewButton)
		{
			NewButton->SetMainButton();
			ButtonsVerticalBox->AddChild(NewButton->MainButton);

			NewButton->OnClickedSignature.AddUniqueDynamic(this, &UBuildingMenu::SetSelectedItem);
		}
	}
	ItemImage->SetVisibility(ESlateVisibility::Hidden);
	BuildButton->SetIsEnabled(false);
	if (!BuildButton->OnClicked.IsBound())
	{
		BuildButton->OnClicked.AddDynamic(this, &UBuildingMenu::BuildItem);
	}
}

void UBuildingMenu::CloseMenu()
{
	Super::CloseMenu();
	ButtonsVerticalBox->ClearChildren();
	ResourcesVerticalBox->ClearChildren();
}

void UBuildingMenu::SetSelectedItem(FBuildingItemInfo& BuildingItem)
{
	BuildButton->SetIsEnabled(true);
	ResourcesVerticalBox->ClearChildren();
	ItemImage->SetBrushFromTexture(BuildingItem.Image);
	ItemImage->SetVisibility(ESlateVisibility::Visible);
	for (auto ResourcesInfo : BuildingItem.NeededResources)
	{
		if (UTextBlock* TextBlock = NewObject<UTextBlock>(this))
		{
			FString TextString = FString::Format(
				TEXT("{0} ({1}/{2})"),
				{ResourcesInfo.Name, CurrentResourceAmount, ResourcesInfo.MinAmount});

			const FText Text = FText::FromString(TextString);
			TextBlock->SetText(Text);
			if (CurrentResourceAmount < ResourcesInfo.MinAmount)
			{
				TextBlock->SetColorAndOpacity(FLinearColor::Red);
			}

			ResourcesVerticalBox->AddChild(TextBlock);
		}
	}
	SelectedItem = BuildingItem;
}

void UBuildingMenu::BuildItem()
{
	OnBuildingItemChosen.Broadcast(SelectedItem.BuildingItemClass);
}
