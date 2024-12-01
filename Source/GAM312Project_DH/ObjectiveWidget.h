// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312PROJECT_DH_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Event called to update meterials objective progress in objective widget
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMatObj(float matsCollected);

	// Event called to update building objective progress in objective widget
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBuildObj(float objectsBuilt);
};
