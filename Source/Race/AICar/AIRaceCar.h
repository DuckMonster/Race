// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "GameFramework/Pawn.h"
#include "AIRaceCar.generated.h"

class URacePlayerStatusWidget;
class ARaceCheckpoint;
UCLASS()
class RACE_API AAIRaceCar : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIRaceCar();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

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

	UPROPERTY()
	TArray<ARaceCheckpoint*> AllCheckPoints;

	int LapsFinished = 0;

	float RaceBeginTime;

	UPROPERTY(VisibleAnywhere)
	ARaceCheckpoint* LastCheckPoint;
	UPROPERTY(VisibleAnywhere)
	ARaceCheckpoint* NextCheckPoint;
	UPROPERTY(VisibleAnywhere)
	FVector Direction = FVector(0.f, 0.f, 0.f);

	FVector Acceleration = FVector(0.f, 0.f, 0.f);
	FVector Velocity = FVector(0.f, 0.f, 0.f);
protected:

	virtual void BeginPlay() override;
	void NavigationCheck();
	void SetDirection( float DeltaTime );
	float Throttle = 200.f;

	void Boost();
	void Move( float DeltaTime );
	void FindNextCheckpoint();
	void AdjustTurnRight( float amount );
	void AdjustAcceleration( float amount );

public:

	virtual void Tick( float DeltaTime ) override;




};
