// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Resource_M.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "ObjectiveWidget.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312PROJECT_DH_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera to be attachted to player character
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;

	// Player stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina = 100.0f;

	// Player resources
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Wood;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Stone;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Berry;

	// Array of resources player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;

	// Array of names for resources player has
	UPROPERTY(EditAnywhere, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	// Hit marker decal
	UPROPERTY(EditAnywhere, Category = "HitMarker")
	UMaterialInterface* hitDecal;

	// Array holding all building parts available to spawn/build
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
	TArray<int> BuildingArray;

	// Building mode indicator
	UPROPERTY()
	bool isBuilding;

	// Controls if snapping to other building parts is enabled in building mode (enabled by default)
	UPROPERTY()
	bool enableBuildingSnapping = true;

	// Parent class of the spawned part
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABuildingPart> BuildPartClass;

	// Builing part that is spawned
	UPROPERTY()
	ABuildingPart* spawnedPart;

	// Reference to player HUD widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerWidget* playerUI;

	// Reference to player objectives widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObjectiveWidget* objWidget;

	// Store number of objects built by player
	UPROPERTY()
	float objectsBuilt;

	// Store number of materials collected by player
	UPROPERTY()
	float matsCollected;

	// Movement functions
	UFUNCTION()
	void MoveForward(float axisValue);

	UFUNCTION()
	void MoveRight(float axisValue);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	// Object interaction
	UFUNCTION()
	void FindObject();

	// Stats related functions
	UFUNCTION(BlueprintCallable)
	void SetHealth(float ammount);

	UFUNCTION(BlueprintCallable)
	void SetHunger(float ammount);

	UFUNCTION(BlueprintCallable)
	void SetStamina(float ammount);

	UFUNCTION()
	void DecreaseStats();

	// Resources related functions
	UFUNCTION()
	void GiveResource(float amount, FString resourceType);

	// Building related functions
	UFUNCTION(BlueprintCallable)
	void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	UFUNCTION(BlueprintCallable)
	void SpawnBuilding(int buildingId, bool& isSuccess);

	UFUNCTION()
	void RotateBuilding();

	UFUNCTION()
	void DestroyBuildingPart();
};
