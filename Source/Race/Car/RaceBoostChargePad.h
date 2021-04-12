#pragma once
#include "GameFramework/Actor.h"
#include "RaceBoostChargePad.generated.h"
class UBoxComponent;
class ARaceCar;

UCLASS()
class ARaceBoostChargePad : public AActor
{
	GENERATED_BODY()

public:
	ARaceBoostChargePad();
	void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CarOverlap;

	UPROPERTY(EditAnywhere)
	float RefuelSpeed = 1.f;

private:
	UFUNCTION()
	void HandleCarBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit);
	UFUNCTION()
	void HandleCarEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TArray<ARaceCar*> OverlappedCars;
};