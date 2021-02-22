// Fill out your copyright notice in the Description page of Project Settings.


#include "WheelComponent.h"

#include "DrawDebugHelpers.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

// Sets default values for this component's properties
UWheelComponent::UWheelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Mesh"));
	mesh->SetupAttachment(this);
}

// Called when the game starts
void UWheelComponent::BeginPlay()
{
	Super::BeginPlay();

	AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(GetOwner());
	if (vehiclePawn)
		vehicleMesh = vehiclePawn->GetMesh();
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}
	// ...
}

bool UWheelComponent::SuspensionForce(float suspensionDistance, float force, float frictionValue)
{
	if (!vehicleMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance"));
		return false;
	}
	FHitResult hit;
	FVector position = GetComponentLocation();
	FVector end = position - (GetUpVector() * suspensionDistance);

	GetWorld()->LineTraceSingleByChannel(hit, position, end, ECC_WorldStatic);

	if (hit.bBlockingHit)
	{
		//HACE FALTA ROTAR LA RUEDA EN EL EJE DE LAS Y HIT NORMAL
		float currentSuspensionCompression = 1 - (hit.Distance / suspensionDistance);
		float springForce = currentSuspensionCompression * force;
		float frictionForce = (currentSuspensionCompression - suspensionCompression);
		frictionForce /= GetWorld()->DeltaTimeSeconds;
		frictionForce *= frictionValue;
		springForce += frictionForce;

		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Red,
		// 	                                 FString::Printf(
		// 		                                 TEXT("%s: %f"), *GetName(),
		// 		                                 currentSuspensionCompression));
		// }

		vehicleMesh->AddForceAtLocation(GetUpVector() * springForce, position, NAME_None);

		suspensionCompression = currentSuspensionCompression;

		

		DrawDebugLine(GetWorld(), position, hit.ImpactPoint, FColor::Green, false, -1, 0, 5);
		return true;
	}
	else
	{
		suspensionCompression = 0;
		DrawDebugLine(GetWorld(), position, end, FColor::Red, false, -1, 0, 5);
		return false;
	}
}
