#pragma once
#include "GameFramework/Actor.h"
#include "RaceCheckpoint.generated.h"
class UBoxComponent;

UCLASS()
class ARaceCheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ARaceCheckpoint();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

private:
	UFUNCTION()
	void HandleBoxOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit);
};
