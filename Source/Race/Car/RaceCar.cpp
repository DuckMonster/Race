#include "RaceCar.h"
#include "Components/BoxComponent.h"

ARaceCar::ARaceCar()
{
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = Box;
}

void ARaceCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("Throttle"), this, &ARaceCar::HandleThrottleInput);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &ARaceCar::HandleTurnRightInput);
}

void ARaceCar::Tick(float DeltaTime)
{
	FVector Acceleration = GetActorForwardVector() * ThrottleInput * AccelerationStrength;
	FVector Gravity = FVector::UpVector * -100.f;

	FVector ParallelVelocity = FVector::DotProduct(Velocity, GetActorForwardVector()) * GetActorForwardVector();
	FVector PerpendicularVelocity = FVector::DotProduct(Velocity, GetActorRightVector()) * GetActorRightVector();

	FVector ParallelFriction = -ParallelVelocity * ParallelFrictionCoefficient;
	FVector PerpendicularFriction = -PerpendicularVelocity * PerpendicularFrictionCoefficient;

	Velocity += (Acceleration + ParallelFriction + PerpendicularFriction + Gravity) * DeltaTime;

	// Iterative delta solving
	const int MaxIterations = 5;
	int IterationCount = 0;

	FVector DeltaToMove = Velocity * DeltaTime;
	while(!DeltaToMove.IsNearlyZero() && ++IterationCount < MaxIterations)
	{
		FHitResult Hit;
		AddActorWorldOffset(DeltaToMove, true, &Hit);
		DeltaToMove -= DeltaToMove * Hit.Time;

		// Collision handling!
		if (Hit.bBlockingHit)
		{
			if (Hit.bStartPenetrating)
			{
				FVector DepenVector = Hit.Normal * (Hit.PenetrationDepth + 0.1f);
				AddActorWorldOffset(DepenVector);
				
			}

			FVector ImpactVelocity = FVector::DotProduct(Velocity, Hit.Normal) * Hit.Normal;
			Velocity -= ImpactVelocity;
			DeltaToMove -= FVector::DotProduct(DeltaToMove, Hit.Normal) * Hit.Normal;
		}
	}

	ensure(IterationCount < MaxIterations);

	// Steering
	float ParallelSpeed = FVector::DotProduct(Velocity, GetActorForwardVector()) / 100.f;
	AddActorWorldRotation(FRotator(0.f, 90.f * ParallelSpeed * TurnRightInput * DeltaTime, 0.f));
}

void ARaceCar::HandleThrottleInput(float Value)
{
	ThrottleInput = Value;
}

void ARaceCar::HandleTurnRightInput(float Value)
{
	TurnRightInput = Value;
}
