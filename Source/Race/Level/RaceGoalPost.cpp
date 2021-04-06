#include "RaceGoalPost.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ARaceGoalPost::ARaceGoalPost()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARaceGoalPost::HandleGoalOverlap);
	RootComponent = Box;
}

void ARaceGoalPost::HandleGoalOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("You goaled!"));

	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, *LevelName, false, TEXT(""));
}
