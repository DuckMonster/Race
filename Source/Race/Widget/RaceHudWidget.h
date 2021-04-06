#pragma once
#include "Blueprint/UserWidget.h"
#include "RaceHudWidget.generated.h"
class URacePlayerStatusWidget;

UCLASS()
class URaceHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	URacePlayerStatusWidget* AddPlayerStatusWidget();
};