// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create default camera and attach it to player mesh head bone
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	// Rotate character along with controller/camera
	PlayerCamComp->bUsePawnControlRotation = true;

	// Intialize resources array with 3 elements in it
	ResourcesArray.SetNum(3);

	// Add names of the resources to the corresponding array
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	// Declare stats timer
	FTimerHandle StatsTimerHandle;

	// Initialize timer and call DecreaseStats function each 2 seconds
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);

	//Action bindings
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
}

void APlayerChar::MoveForward(float axisValue)
{
	// Get player yaw rotation (this avoids problems when looking down/up)
	FRotator YawOnlyRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	// Get forward vector of the character using rotation matrix
	FVector Direction = FRotationMatrix(YawOnlyRotation).GetScaledAxis(EAxis::X);

	// Move character in the direction of the forward vector multiplied by axis value
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::MoveRight(float axisValue)
{
	// Get player yaw rotation (this avoids problems when looking down/up)
	FRotator YawOnlyRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	// Get right vector of the character using rotation matrix
	FVector Direction = FRotationMatrix(YawOnlyRotation).GetScaledAxis(EAxis::Y);

	// Move character in the direction of the right vector multiplied by axis value
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::StartJump()
{
	// Utilize built-in jump function
	bPressedJump = true;
}

void APlayerChar::StopJump()
{
	// Utilize built-in jump function
	bPressedJump = false;
}

void APlayerChar::FindObject()
{
	// Setup hitscan: use player camera position as starting point, view direction multiplied by 800 (length of the line) as line trace direction, and calculate end location using theses values
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	// Collsion query parameters
	FCollisionQueryParams QueryParams;
	// Ignore hits on player
	QueryParams.AddIgnoredActor(this);
	// Include complex collisions
	QueryParams.bTraceComplex = true;
	// Return face index of the object hit
	QueryParams.bReturnFaceIndex = true;

	// Perform line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		// Cast object hit to resource
		AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

		// If player has stamina attempt to collect resource
		if (Stamina > 5.0f && HitResource)
		{
			// Get name and value of the hit resource
			FString hitName = HitResource->resourceName;
			int resourceValue = HitResource->resourceAmount;

			if (HitResource->totalResource > resourceValue)
			{
				GiveResource(resourceValue, hitName);

				// Decrease resource reserve by amount retrieved
				HitResource->totalResource = HitResource->totalResource - resourceValue;

				// Debug message to check notify that resource was collected
				check(GEngine != nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));
				}

				// Spawn decal of size 10 at line trace hit location
				UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);
				
				// Consume stamina
				SetStamina(-5.0f);
			}
			else
			{
				HitResource->Destroy();

				// Debug message to check notify that resource was destroyed
				check(GEngine != nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
				}
			}
		}
	}
}

// Function used to update player health
void APlayerChar::SetHealth(float amount)
{
	// If adding the amount would cause the health stat to exceed 100, increase health only up to the maximum of 100. Else just add amount to health
	if (Health + amount > 100)
	{
		Health = 100.0f;
	}
	else
	{
		Health = Health + amount;
	}
}

// Function used to update player hunger
void APlayerChar::SetHunger(float amount)
{
	// If adding the amount would cause the hunger stat to exceed 100, increase hunger only up to the maximum of 100. Else just add amount to hunger
	if (Hunger + amount > 100)
	{
		Hunger = 100.0f;
	}
	else
	{
		Hunger = Hunger + amount;
	}
}

// Function used to update player stamina
void APlayerChar::SetStamina(float amount)
{
	// If adding the amount would cause the stamina stat to exceed 100, increase stamina only up to the maximum of 100. Else just add amount to stamina
	if (Stamina + amount > 100)
	{
		Stamina = 100.0f;
	}
	else
	{
		Stamina = Stamina + amount;
	}
}

// This function decreases player stats each time it's called
void APlayerChar::DecreaseStats()
{
	if (Hunger <= 0)
	{
		// If player is starving start damaging health
		SetHealth(-3.0f);
	}
	else
	{
		// If hunger level is not 0, decrease hunger level each time function is called
		SetHunger(-1.0f);
	}

	// Each time function is called restore stamina
	SetStamina(10.0f);
}

// Function used to update player resources
void APlayerChar::GiveResource(float amount, FString resourceType)
{
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}
	else if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}
	else if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

