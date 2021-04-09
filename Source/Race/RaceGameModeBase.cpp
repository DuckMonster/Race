#include "RaceGameModeBase.h"
#include "Widget/RaceHudWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RaceCheckpoint.h"

void ARaceGameModeBase::StartPlay()
{
	// Add kewl widget
	HudWidget =	CreateWidget<URaceHudWidget>(GetWorld(), WidgetClass);
	HudWidget->AddToViewport();

	// Count number of checkpoints
	TArray<AActor*> Checkpoints;
	UGameplayStatics::GetAllActorsOfClass(this, ARaceCheckpoint::StaticClass(), Checkpoints); 

	NumCheckpoints = Checkpoints.Num();

	Super::StartPlay();
}
