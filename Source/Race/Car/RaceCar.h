#pragma once
#include "GameFramework/Pawn.h"
#include "RaceCar.generated.h"
class UBoxComponent;

UCLASS()
class ARaceCar : public APawn
{
	GENERATED_BODY()

public:
	ARaceCar();
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Tick(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

	UPROPERTY(Category = "Car Physics", EditAnywhere)
	float AccelerationStrength = 200.f;

	UPROPERTY(Category = "Car Physics", EditAnywhere)
	float ParallelFrictionCoefficient = 1.2f;

	UPROPERTY(Category = "Car Physics", EditAnywhere)
	float PerpendicularFrictionCoefficient = 5.1f;

private:
	void HandleThrottleInput(float Value);
	void HandleTurnRightInput(float Value);

	float ThrottleInput;
	float TurnRightInput;

	FVector Velocity;
};