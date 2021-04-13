#include "RaceCheckpoint.h"
#include "Components/BoxComponent.h"
#include "Car/RaceCar.h"
#include "AICar/AIRaceCar.h"
#include "Widget/RacePlayerStatusWidget.h"

ARaceCheckpoint::ARaceCheckpoint(){
	RootComponent = Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Root);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ARaceCheckpoint::HandleBoxOverlap);
}

void ARaceCheckpoint::HandleBoxOverlap( UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit ){
	auto Car = Cast<ARaceCar>(OtherActor);
	if ( !Car ){

		auto AICar = Cast<AAIRaceCar>(OtherActor);
		if ( !AICar ){
			return;
		}
		if ( AICar->PassedCheckpoints.Contains(this) )
			return;

		AICar->PassedCheckpoints.Add(this);
		AICar->StatusWidget->SetCheckpointsPassed(AICar->PassedCheckpoints.Num());
		AICar->LastCheckPoint = this;
		CheckPointPriority -= 8.f;
		return;
	}

	if ( Car->PassedCheckpoints.Contains(this) )
		return;

	Car->PassedCheckpoints.Add(this);
	Car->StatusWidget->SetCheckpointsPassed(Car->PassedCheckpoints.Num());

}
