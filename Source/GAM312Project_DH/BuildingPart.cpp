// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPart.h"
#include <string>

// Sets default values
ABuildingPart::ABuildingPart()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Mesh and PivotArrow components with default values
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PivotArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Pivot Arrow"));

	// Set pivot arrow to be the root component 
	RootComponent = PivotArrow;

	// Attach mesh component to the pivot arrow
	Mesh->SetupAttachment(PivotArrow);

	for (int i = 0; i < 8; i++)
	{
		UArrowComponent* attachmentSocket = CreateDefaultSubobject<UArrowComponent>(FName(*FString::Printf(TEXT("AttachmentSocket%d"), i)));
		attachmentSocket->SetupAttachment(RootComponent);
		AttachmentSockets.Add(attachmentSocket);
	}
}

// Called when the game starts or when spawned
void ABuildingPart::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABuildingPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PivotArrow->GetComponentLocation();
}

void ABuildingPart::DestroyProcedure()
{
	// Check if part can be destroyed
	if (CanBeDestroyed)
	{
		CanBeDestroyed = false;

		// Check for building parts above attachment sockets
		for (UArrowComponent* attachmentSocket : AttachmentSockets)
		{
			// Setup hitscan: use attachment socket position starting point, up direction multiplied by 50 (length of the line) as line trace direction, and calculate end location using theses values
			FHitResult HitResult;
			FVector StartLocation = attachmentSocket->GetComponentLocation();
			FVector Direction = attachmentSocket->GetUpVector() * 50.0f;
			FVector EndLocation = StartLocation + Direction;

			// Collsion query parameters
			FCollisionQueryParams QueryParams;
			// Ignore hits on self
			QueryParams.AddIgnoredActor(this);
			// Include complex collisions
			QueryParams.bTraceComplex = true;
			// Return face index of the object hit
			QueryParams.bReturnFaceIndex = true;

			// Draw debug
			//const FName TraceTag("TraceTag2");
			//GetWorld()->DebugDrawTraceTag = TraceTag;
			//QueryParams.TraceTag = TraceTag;

			// Destroy any building parts above 
			if (GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(2.0f), QueryParams))
			{
				// Cast object hit to building part
				ABuildingPart* BuildingPartAbove = Cast<ABuildingPart>(HitResult.GetActor());
				// Trigger destroy procedure in part above
				BuildingPartAbove->DestroyProcedure();
			}
		}

		// Set this part to destroy in 3 seconds
		SetLifeSpan(3.0f);
	}
}
