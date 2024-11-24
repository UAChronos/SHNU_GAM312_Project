// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312PROJECT_DH_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Event called to update bars in player HUD
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBars(float Health, float Hunger, float Stamina);
};
