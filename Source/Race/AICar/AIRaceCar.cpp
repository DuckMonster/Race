// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIRaceCar.h"


#include "ChaosInterfaceWrapperCore.h"
#include "DrawDebugHelpers.h"



#include "RaceGameModeBase.h"

#include "Car/RaceCar.h"
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
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));

	Sphere->InitSphereRadius(30.f);
	Sphere->SetupAttachment(Box);

}

// Called when the game starts or when spawned
void AAIRaceCar::BeginPlay(){
	Super::BeginPlay();

	auto* GameMode = Cast<ARaceGameModeBase>(UGameplayStatics::GetGameMode(this));
	StatusWidget = GameMode->HudWidget->AddPlayerStatusWidget();
	StatusWidget->SetTotalCheckpointNum(GameMode->NumCheckpoints);

	RaceBeginTime = GetWorld()->TimeSeconds;
	StatusWidget->SetRaceBeginTime(RaceBeginTime);

	for ( AActor* Actor : GameMode->Checkpoints ){
		ARaceCheckpoint* CheckPoint = Cast<ARaceCheckpoint>(Actor);
		AllCheckPoints.Add(CheckPoint);

	}
	for ( int i = 0; i < AllCheckPoints.Num(); ++i )
		for ( int j = 0; j < AllCheckPoints.Num() - 1; ++j )
			if ( AllCheckPoints[i]->Index < AllCheckPoints[j]->Index )
				AllCheckPoints.Swap(i, j);
	UE_LOG(LogTemp, Log, TEXT("FirstIndex '%i'"), AllCheckPoints[0]->Index);
	NextCheckPoint = AllCheckPoints[0];

}

// Called every frame
void AAIRaceCar::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);
	NavigationCheck(DeltaTime);
	SetDirection();
	Move(DeltaTime);

}

void AAIRaceCar::NavigationCheck( float DeltaTime ){

	TArray<AActor*> OverlapActors;
	Sphere->GetOverlappingActors(OverlapActors);
	OverlapActors.RemoveSingle(this);



	OverlapActors.Shrink();
	if ( OverlapActors.Num() <= 0 ){
		Boost();
		return;
	}

	for ( AActor* OverlapActor : OverlapActors ){
		if ( OverlapActor == this )
			continue;
		FVector Origin;
		FVector BoxExtents;
		OverlapActor->GetActorBounds(false, Origin, BoxExtents, false);
		float Dot = FVector::DotProduct(GetActorForwardVector(), Origin.GetSafeNormal());

		if ( Dot < 1 / sqrt(2.f) ){
			const FVector AdjustAmount = Origin + BoxExtents * 2.f;
			if ( OverlapActor->GetHorizontalDistanceTo(this) < 20.f ){
				AdjustThrottle(0.6f, DeltaTime);
				if ( FVector::DotProduct(Velocity, Origin) < 1 / sqrt(2.f) ){
					AdjustThrottle(0.3f, DeltaTime);
					AdjustDirection(AdjustAmount);
				}
			}
			AdjustDirection(BoxExtents);
		}
		else{
			AdjustThrottle(1.f, DeltaTime);
			FHitResult TraceHit;
			TArray<AActor*> ActorsToIgnore;
			GetAllChildActors(ActorsToIgnore, true);
			if ( UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorForwardVector(), GetActorForwardVector() * 300.f, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, TraceHit, true) ){
				if ( TraceHit.Distance > 50.f ){
					Boost();
				}
			}
			else{
				Boost();
			}
		}
	}
	OverlapActors.Empty();

}


void AAIRaceCar::SetDirection(){
	FindNextCheckpoint();
	Direction = (NextCheckPoint->GetActorLocation() - GetActorLocation());
	Direction.Normalize();

}
void AAIRaceCar::ResetCheckPoints(){
	PassedCheckpoints.Empty();
	NextCheckPoint = AllCheckPoints[0];
}


void AAIRaceCar::AdjustThrottle( float Value, float DeltaTime ){
	ThrottleInput = FMath::Lerp(ThrottleInput, FMath::Clamp(Value, -1.f, 1.f), DeltaTime);
}

void AAIRaceCar::Boost(){
	if ( BoostTime <= 0.f )
		if ( !bIsBoosting )
			bIsBoosting = true;

}
void AAIRaceCar::Move( float DeltaTime ){

	FVector Acceleration = Direction * ThrottleInput * Force;
	if ( bIsBoosting && BoostTime > 0.f ){
		Acceleration = Direction * Force * 3.f;
		BoostTime -= DeltaTime;
	}
	StatusWidget->SetBoostPercent(BoostTime / 2.f);
	FVector Gravity = FVector::UpVector * -100.f;

	FVector ParallelVelocity = FVector::DotProduct(Velocity, GetActorForwardVector()) * GetActorForwardVector();
	FVector PerpendicularVelocity = FVector::DotProduct(Velocity, GetActorRightVector()) * GetActorRightVector();

	FVector ParallelFriction = -ParallelVelocity * ParallelFrictionCoefficient;
	FVector PerpendicularFriction = -PerpendicularVelocity * PerpendicularFrictionCoefficient;
	Velocity += (Acceleration + Direction + ParallelFriction + PerpendicularFriction + Gravity) * DeltaTime;

	//Iterative delta solving
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
				FVector Depenetration = Hit.Normal * (Hit.PenetrationDepth + 0.1f);

				AddActorWorldRotation(Depenetration.ToOrientationRotator() * -1.f);

				AddActorWorldOffset(Depenetration);

			}

			FVector ImpactVelocity = FVector::DotProduct(Velocity, Hit.Normal) * Hit.Normal;
			Velocity -= ImpactVelocity;
			DeltaToMove -= FVector::DotProduct(DeltaToMove, Hit.Normal) * Hit.Normal;
		}

	}
	// Rotate Body

	SetActorRotation(Velocity.ToOrientationRotator());
}

void AAIRaceCar::AdjustDirection( FVector Value ){
	if ( Value.Y < 0 )
		Direction.Y -= Value.Y;
	else
		Direction.Y += Value.Y;
	if ( Value.X < 0 )
		Direction.X -= Value.X;
	else
		Direction.X += Value.X;
	Direction.Normalize(1.f);


}

void AAIRaceCar::FindNextCheckpoint(){
	for ( ARaceCheckpoint* RaceCheckpoint : AllCheckPoints ){

		if ( PassedCheckpoints.Contains(RaceCheckpoint) )
			continue;
		NextCheckPoint = RaceCheckpoint;
		break;
	}
	if ( NextCheckPoint == LastCheckPoint )
		ResetCheckPoints();
	DrawDebugSphere(GetWorld(), NextCheckPoint->GetActorLocation(), 5.f, 10, FColor::Green);
}
