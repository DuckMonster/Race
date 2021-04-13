#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaceGameModeBase.generated.h"
class URaceHudWidget;

UCLASS()
class RACE_API ARaceGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void StartPlay() override;

	int NumCheckpoints;

	UPROPERTY(Category="Race", EditDefaultsOnly)
	int NumLaps = 3;
	UPROPERTY()
	TArray<AActor*> Checkpoints;
	
	UPROPERTY(Category="HUD", EditDefaultsOnly)
	TSubclassOf<URaceHudWidget> WidgetClass;
	URaceHudWidget* HudWidget;
};
