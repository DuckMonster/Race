// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIRaceCar.h"
#include "DrawDebugHelpers.h"
#include "RaceGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RaceCheckpoint.h"
#include "Widget/RaceHudWidget.h"
#include "Widget/RacePlayerStatusWidget.h"


// Sets default values
AAIRaceCar::AAIRaceCar(){
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = Box;


}

// Called when the game starts or when spawned
void AAIRaceCar::BeginPlay(){
	Super::BeginPlay();

	auto* GameMode = Cast<ARaceGameModeBase>(UGameplayStatics::GetGameMode(this));
	StatusWidget = GameMode->HudWidget->AddPlayerStatusWidget();
	StatusWidget->SetTotalCheckpointNum(GameMode->NumCheckpoints);

	RaceBeginTime = GetWorld()->TimeSeconds;
	StatusWidget->SetRaceBeginTime(RaceBeginTime);
	for ( AActor* actor : GameMode->Checkpoints ){
		ARaceCheckpoint* checkPoint = Cast<ARaceCheckpoint>(actor);
		AllCheckPoints.Add(checkPoint);
	}
}
void AAIRaceCar::NavigationCheck(){
	FHitResult HitResult;

	//CheckForward
	if ( GetWorld()->SweepSingleByObjectType(HitResult, GetActorForwardVector(), GetActorForwardVector() * 100.f, GetActorRotation().Quaternion(), FCollisionObjectQueryParams::DefaultObjectQueryParam, FCollisionShape::MakeBox(FVector(0.5f))) ){
		//TODO DEBUG REMOVE
		FString msg = "HIT OBJECT: ";
		msg.Append(HitResult.GetActor()->GetName());
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, msg);
		
		 msg = "HitDistance: ";
		msg.AppendInt(HitResult.Distance);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, msg);
		
		if ( HitResult.Distance > 5.f ){
			AdjustAcceleration(HitResult.Distance);
		}
		else if ( HitResult.Distance < 5.f ){
			//	AdjustTurnRight(HitResult.Distance);
			AdjustAcceleration(HitResult.Distance * 0.5f);
		}
	}
	else{

		AdjustAcceleration(200.f);

	}


}
void AAIRaceCar::SetDirection( float DeltaTime ){

	Direction = GetActorLocation() - NextCheckPoint->GetActorLocation();
	DrawDebugLine(GetWorld(), GetActorForwardVector(), Direction, FColor::Red);

}

void AAIRaceCar::AdjustTurnRight( float Value ){
	FString msg = "Turn Right: ";
	msg.AppendInt(Value);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, msg);
}
void AAIRaceCar::AdjustAcceleration( float Value ){
	AccelerationStrength = 200.f;
	Acceleration = GetActorForwardVector() * Value;
}
void AAIRaceCar::Boost(){
	bIsBoosting = true;

}
void AAIRaceCar::Move( float DeltaTime ){
	StatusWidget->SetBoostPercent(BoostTime / 2.f);
	FindNextCheckpoint();
	SetDirection(DeltaTime);
	NavigationCheck();


	FVector Gravity = FVector::UpVector * -100.f;

	FVector ParallelVelocity = FVector::DotProduct(Velocity, GetActorForwardVector()) * GetActorForwardVector();
	FVector PerpendicularVelocity = FVector::DotProduct(Velocity, GetActorRightVector()) * GetActorRightVector();

	FVector ParallelFriction = -ParallelVelocity * ParallelFrictionCoefficient;
	FVector PerpendicularFriction = -PerpendicularVelocity * PerpendicularFrictionCoefficient;

	Velocity += (Acceleration + ParallelFriction + PerpendicularFriction + Gravity) * DeltaTime;
	if ( bIsBoosting && BoostTime > 0.f ){
		Acceleration = GetActorForwardVector() * AccelerationStrength * 3.f;
		BoostTime -= DeltaTime;
	}

	const int MaxIterations = 5;
	int IterationCount = 0;

	FVector DeltaToMove = Velocity * DeltaTime;
	while ( !DeltaToMove.IsNearlyZero() && ++IterationCount < MaxIterations ){
		FHitResult Hit;
		AddActorWorldOffset(DeltaToMove, true, &Hit);
		DeltaToMove -= DeltaToMove * Hit.Time;

		// Collision handling!
		if ( Hit.bBlockingHit ){
			if ( Hit.bStartPenetrating ){
				FVector DepenVector = Hit.Normal * (Hit.PenetrationDepth + 0.1f);
				AddActorWorldOffset(DepenVector);

			}

			FVector ImpactVelocity = FVector::DotProduct(Velocity, Hit.Normal) * Hit.Normal;
			Velocity -= ImpactVelocity;
			DeltaToMove -= FVector::DotProduct(DeltaToMove, Hit.Normal) * Hit.Normal;
		}
	}

	// Steering
	float ParallelSpeed = FVector::DotProduct(Velocity, GetActorForwardVector()) / 100.f;
	FRotator rotation = FRotator(0.f, Direction.Y * ParallelSpeed * DeltaTime, 0.f);
	AddActorWorldRotation(rotation);
}

void AAIRaceCar::FindNextCheckpoint(){
	for ( ARaceCheckpoint* checkPoint : AllCheckPoints ){
		if ( PassedCheckpoints.Contains(checkPoint) ){
			continue;
		}
		NextCheckPoint = checkPoint;
		break;
	}

}


// Called every frame
void AAIRaceCar::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);
	Move(DeltaTime);

}
