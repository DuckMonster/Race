#pragma once

#include "GameFramework/Actor.h"
#include "BoostRefill.generated.h"

class UBoxComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UDecalComponent;

UCLASS()
class ABoostRefill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoostRefill();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* HitBox;

	UPROPERTY(VisibleAnywhere)
	UDecalComponent* DecalComp;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* Parent;

	void BeginPlay() override;

	UMaterialInstanceDynamic* MatInst;

	
};
