// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


#include "GameFramework/Pawn.h"

#include "Serialization/TraceReferences.h"

#include "AIRaceCar.generated.h"

class URacePlayerStatusWidget;
class ARaceCheckpoint;
class ALandscape;
UCLASS()
class RACE_API AAIRaceCar : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIRaceCar();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(Category = "Car Physics", EditAnywhere)
	float Force = 500.f;

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
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(Category = "Car Physics", EditAnywhere)
	FVector Direction;
	UPROPERTY(Category = "Car Physics", EditAnywhere)
	FVector Velocity;;
	float ThrottleInput = 1.f;
	void FindNextCheckpoint();
	void ResetCheckPoints();
	void NavigationCheck( float DeltaTime );

protected:
	virtual void BeginPlay() override;
	void SetDirection();
	void Boost();
	void Move( float DeltaTime );
	void AdjustDirection( FVector Value );
	void AdjustThrottle( float Value, float DeltaTime );
public:

	virtual void Tick( float DeltaTime ) override;




};
