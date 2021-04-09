// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceCarFPCamera.h"

void URaceCarFPCamera::BeginPlay(){
	RotateWithPlayer();
}
void URaceCarFPCamera::OnEndOfFrameUpdateDuringTick(){
	RotateWithPlayer();
}
void URaceCarFPCamera::RotateWithPlayer(){
	FTransform playerTransform = GetAttachmentRootActor()->GetTransform();
	GetRelativeTransform().SetTranslation(playerTransform.GetTranslation());
}
