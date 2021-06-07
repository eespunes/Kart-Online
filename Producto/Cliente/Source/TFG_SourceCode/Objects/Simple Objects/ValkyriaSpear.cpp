// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "ValkyriaSpear.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void AValkyriaSpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (used)
	{
		SetActorLocation(GetActorLocation() + forward * speed, true);
		if (timer >= ttl)
			Destroy();
		timer += DeltaTime;
	}
}

void AValkyriaSpear::UseObject()
{
	Super::UseObject();
	SetActorLocation(vehicle->GetActorLocation());
	SetActorRotation(vehicle->GetActorRotation());

	forward = vehicle->GetForward();

	used = true;

	Stop();
}

void AValkyriaSpear::SetUp()
{
	Super::SetUp();
	mesh->SetSimulatePhysics(false);
}

void AValkyriaSpear::Stop()
{
}
