#pragma once

#include "Camera/CameraComponent.h"

#include "GameFramework/Pawn.h"
#include "RaceCar.generated.h"
class UBoxComponent;
class URacePlayerStatusWidget;
class ARaceCheckpoint;
class AIComponent;

UCLASS()
class ARaceCar : public APawn {
	GENERATED_BODY()

public:
	ARaceCar();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) override;
	virtual void PossessedBy( AController* NewController ) override;
	void Move( float DeltaTime );

	virtual void Tick( float DeltaTime ) override;

	UPROPERTY(EditDefaultsOnly)
	ACameraActor* CameraActor;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(Category = "Car Physics", EditAnywhere)
	float AccelerationStrength = 200.f;

	UPROPERTY(Category = "Car Physics", EditAnywhere)
	float ParallelFrictionCoefficient = 1.2f;

	UPROPERTY(Category = "Car Physics", EditAnywhere)
	float PerpendicularFrictionCoefficient = 5.1f;

	float BoostTime = 2.f;
	bool bIsBoosting = false;

	UPROPERTY(VisibleAnywhere)
	URacePlayerStatusWidget* StatusWidget;

	UPROPERTY()
	TSet<ARaceCheckpoint*> PassedCheckpoints;
	int LapsFinished = 0;

	float RaceBeginTime;
	FVector Velocity;


private:
	void HandleThrottleInput( float Value );
	void HandleTurnRightInput( float Value );

	void HandleBoostPressed();
	void HandleBoostReleased();

	void HandleSwitchCameraInput();

	float ThrottleInput;
	float TurnRightInput;


};
