// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAM312PROJECT_DH_API AResource_M : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_M();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Resource properties
	UPROPERTY(EditAnywhere)
	FString resourceName = "Wood";

	// Resource increment value (how much player gets upon resource collection)
	UPROPERTY(EditAnywhere)
	int resourceAmount = 5;

	// Max amount of resource that can be collected before source is depleted
	UPROPERTY(EditAnywhere)
	int totalResource = 100;

	// Properties related to the resource name
	UPROPERTY()
	FText tempText;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* ResourceNameTxt;

	// Mesh used for the resource
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
};
