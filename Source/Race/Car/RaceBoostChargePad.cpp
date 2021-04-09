#include "RaceBoostChargePad.h"
#include "Components/BoxComponent.h"
#include "RaceCar.h"

ARaceBoostChargePad::ARaceBoostChargePad()
{
	PrimaryActorTick.bCanEverTick = true;
	CarOverlap = CreateDefaultSubobject<UBoxComponent>("Box");
	RootComponent = CarOverlap;

	CarOverlap->OnComponentBeginOverlap.AddDynamic(this, &ARaceBoostChargePad::HandleCarBeginOverlap);
	CarOverlap->OnComponentEndOverlap.AddDynamic(this, &ARaceBoostChargePad::HandleCarEndOverlap);
}

void ARaceBoostChargePad::Tick(float DeltaTime)
{
	for(auto* Car : OverlappedCars)
		Car->BoostTime += RefuelSpeed * DeltaTime;
}

void ARaceBoostChargePad::HandleCarBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{
	auto* Car = Cast<ARaceCar>(OtherActor);
	if (!Car)
		return;

	OverlappedCars.AddUnique(Car);
}

void ARaceBoostChargePad::HandleCarEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto* Car = Cast<ARaceCar>(OtherActor);
	if (!Car)
		return;

	OverlappedCars.Remove(Car);
}

