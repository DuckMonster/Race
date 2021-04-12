#pragma once
#include "Blueprint/UserWidget.h"
#include "RacePlayerStatusWidget.generated.h"

UCLASS()
class URacePlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetBoostPercent(float BoostPercent);

	UFUNCTION(BlueprintImplementableEvent)
	void SetCheckpointsPassed(int CheckpointsPassed);

	UFUNCTION(BlueprintImplementableEvent)
	void SetTotalCheckpointNum(int TotalCheckpointNum);
};