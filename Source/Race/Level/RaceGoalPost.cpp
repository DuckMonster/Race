#include "RaceGoalPost.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Car/RaceCar.h"
#include "RaceCheckpoint.h"
#include "RaceGameModeBase.h"
#include "Widget/RacePlayerStatusWidget.h"

ARaceGoalPost::ARaceGoalPost()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARaceGoalPost::HandleGoalOverlap);
	RootComponent = Box;
}

void ARaceGoalPost::HandleGoalOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{
	auto Car = Cast<ARaceCar>(OtherActor);
	if (!Car)
		return;

	auto GameMode = Cast<ARaceGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (Car->PassedCheckpoints.Num() != GameMode->NumCheckpoints)
		return;

	Car->PassedCheckpoints.Empty();
	Car->StatusWidget->SetCheckpointsPassed(0);

	Car->LapsFinished++;

	if (Car->LapsFinished >= GameMode->NumLaps)
	{
		FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
		UGameplayStatics::OpenLevel(this, *LevelName, false, TEXT(""));
	}
}
