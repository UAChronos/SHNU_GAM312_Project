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
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

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
}

