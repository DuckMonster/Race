#pragma once
#include "Camera/CameraComponent.h"

#include "GameFramework/Pawn.h"
#include "RaceCar.generated.h"
class UBoxComponent;
class URacePlayerStatusWidget;

UCLASS()
class ARaceCar : public APawn {
	GENERATED_BODY()

public:
	ARaceCar();
	void BeginPlay() override;
	void SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) override;
	void PossessedBy( AController* NewController ) override;

	void Tick( float DeltaTime );

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
	APlayerCameraManager* PlayerCameraManager;

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

	UPROPERTY()
	URacePlayerStatusWidget* StatusWidget;

private:
	void HandleThrottleInput( float Value );
	void HandleTurnRightInput( float Value );

	void HandleBoostPressed();
	void HandleBoostReleased();

	float ThrottleInput;
	float TurnRightInput;

	FVector Velocity;
};
