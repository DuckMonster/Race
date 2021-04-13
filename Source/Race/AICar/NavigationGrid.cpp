// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationGrid.h"

#include "DrawDebugHelpers.h"





// Sets default values
ANavigationGrid::ANavigationGrid(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WORLDBOX"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(GridSize);



}

// Called when the game starts or when spawned
void ANavigationGrid::BeginPlay(){
	Super::BeginPlay();
	for ( int x = 0; x < GridSize.X; ++x ){

		for ( int y = 0; y < GridSize.Y; ++y ){
			Points.Add(FVector(x, y, 0.f));



		}
	}
	DrawDebugPoint(GetWorld(), Points.Last(), 1.f, FColor::Green);
}
// Called every frame
void ANavigationGrid::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);
	DrawDebugBox(this->GetWorld(), GetActorLocation(), BoxComponent->GetScaledBoxExtent(), FColor::Red);

}
