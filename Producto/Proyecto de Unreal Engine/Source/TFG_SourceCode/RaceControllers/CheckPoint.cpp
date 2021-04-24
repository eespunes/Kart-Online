// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "CheckPoint.h"


#include "DrawDebugHelpers.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	leftLocation = GetActorLocation() + GetActorRightVector() * AiOffsetLeft;
	rightLocation = GetActorLocation() + GetActorRightVector() * AiOffsetRight;


	DrawDebugLine(GetWorld(), leftLocation, leftLocation + GetActorUpVector() * 1000, FColor::Purple, false,
	              1000000,
	              0, 15);
	DrawDebugLine(GetWorld(), rightLocation, rightLocation + GetActorUpVector() * 1000,
	              FColor::Purple, false, 1000000,
	              0, 15);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000,
	              FColor::Blue, false, 1000000,
	              0, 15);

	plane = FPlane(GetActorLocation(), GetActorForwardVector());
}

int ACheckPoint::GetPosition() const
{
	return position;
}

FVector ACheckPoint::GetLocation()
{
	return FVector(
		FMath::RandRange(leftLocation.X, rightLocation.X),
		FMath::RandRange(leftLocation.Y, rightLocation.Y),
		FMath::RandRange(leftLocation.Z, rightLocation.Z)
	);
}

bool ACheckPoint::IsPlayerInPlane(FVector playerLocation)
{
	return plane.PlaneDot(playerLocation) < 0;
}
