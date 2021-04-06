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

	UPROPERTY(Category="HUD", EditDefaultsOnly)
	TSubclassOf<URaceHudWidget> WidgetClass;
	URaceHudWidget* HudWidget;
};
