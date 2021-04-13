// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"

#include "GameFramework/Actor.h"

#include "NavigationGrid.generated.h"

UCLASS()
class RACE_API ANavigationGrid : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANavigationGrid();
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;
	

	UPROPERTY(EditAnywhere)
	FVector GridSize = FVector(100.f);
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Points;
	UPROPERTY(VisibleAnywhere)
	int PosX;
	UPROPERTY(VisibleAnywhere)
	int PosY;
	UPROPERTY(VisibleAnywhere)
	FVector CurrentPosition;
	UPROPERTY(VisibleAnywhere)
	FVector Destination;
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Path;
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> AlreadyVisited;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick( float DeltaTime ) override;



};
