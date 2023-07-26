#include "WidgetMenuBase.h"

void UWidgetMenuBase::OpenMenu()
{
	SetVisibility(ESlateVisibility::Visible);
	SetIgnorePlayerMovement.Broadcast(true);
}

void UWidgetMenuBase::CloseMenu()
{
	SetVisibility(ESlateVisibility::Collapsed);
	SetIgnorePlayerMovement.Broadcast(false);
}
