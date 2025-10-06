// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "BuildingPart.generated.h"

UCLASS()
class GAM312PROJECT_DH_API ABuildingPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Building part mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	// Pivot object used as origin for building part
	UPROPERTY(EditAnywhere)
    UArrowComponent* PivotArrow;

	// Array of arrows used as building part attachment sockets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment Sockets")
	TArray<UArrowComponent*> AttachmentSockets;
};
