// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "RaceCarFPCamera.generated.h"

/**
 * 
 */
UCLASS()
class RACE_API URaceCarFPCamera : public UCameraComponent {
	GENERATED_BODY()
	void BeginPlay() override;

	virtual void OnEndOfFrameUpdateDuringTick() override;

	UFUNCTION(BlueprintCallable)
	void RotateWithPlayer();

};
