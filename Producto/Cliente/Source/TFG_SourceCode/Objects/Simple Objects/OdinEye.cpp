// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "OdinEye.h"

#include <algorithm>


#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void AOdinEye::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOdinEye::UseObject()
{
	Super::UseObject();

	ARacingLevel* level = Cast<ARacingLevel>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));

	FVector position;
	FRotator rotation;
	ACheckPoint* checkPoint;


	int32 currentPosition = vehicle->GetRaceComponent()->GetPosition() - 2;
	if (currentPosition < 0)
	{
		checkPoint = vehicle->GetRaceComponent()->GetCurrentCheckPoint();
		position = checkPoint->GetActorLocation();
		rotation = checkPoint->GetActorRotation();
		FVector rotationEuler = rotation.Euler();
		rotation = FRotator::MakeFromEuler(FVector(rotationEuler.X, rotationEuler.Y, rotationEuler.Z - 180));
	}
	else
	{
		int32 expectedCheckpoint = level->GetVehicle(currentPosition)->
		                                  GetCheckpointPosition();
		int32 leaderExpectedCheckpoint = level->GetVehicle(0)->GetCheckpointPosition();

		if (expectedCheckpoint == leaderExpectedCheckpoint)
		{
			expectedCheckpoint = vehicle->GetRaceComponent()->GetCheckpointPosition();
		}

		int32 difference = leaderExpectedCheckpoint - expectedCheckpoint;
		difference *= FMath::RandRange(min, max);
		difference = difference == 0 ? 1 : difference;
		expectedCheckpoint += difference;

		checkPoint = level->GetCheckpoint(expectedCheckpoint);
		position = checkPoint->GetActorLocation();
		rotation = checkPoint->GetActorRotation();
		FVector rotationEuler = rotation.Euler();
		rotation = FRotator::MakeFromEuler(FVector(rotationEuler.X, rotationEuler.Y, rotationEuler.Z - 180));
	}

	vehicle->GetRaceComponent()->SetCheckpoint(checkPoint);
	vehicle->InstantiateParticle(particle);

	vehicle->SetActorLocation(position + offset, false, nullptr, ETeleportType::TeleportPhysics);
	vehicle->SetActorRotation(rotation, ETeleportType::TeleportPhysics);

	Stop();
	Destroy();
}

void AOdinEye::SetUp()
{
	Super::SetUp();
}

void AOdinEye::Stop()
{
	Super::Stop();
	Destroy();
}
