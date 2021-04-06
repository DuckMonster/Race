#pragma once
#include "GameFramework/Actor.h"
#include "RaceGoalPost.generated.h"
class UBoxComponent;

UCLASS()
class ARaceGoalPost : public AActor
{
	GENERATED_BODY()

public:
	ARaceGoalPost();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

	UFUNCTION()
	void HandleGoalOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit);
};
