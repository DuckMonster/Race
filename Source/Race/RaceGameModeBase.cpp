#include "RaceGameModeBase.h"
#include "Widget/RaceHudWidget.h"

void ARaceGameModeBase::StartPlay()
{
	// Add kewl widget
	HudWidget =	CreateWidget<URaceHudWidget>(GetWorld(), WidgetClass);
	HudWidget->AddToViewport();

	Super::StartPlay();
}
