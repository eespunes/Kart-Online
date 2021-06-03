// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "LokiTrick.h"


#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void ALokiTrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (used)
	{
		if (timer >= duration)
			Stop();
		timer += DeltaTime;
	}
}

void ALokiTrick::UseObject()
{
	Super::UseObject();

	ARacingLevel* level = Cast<ARacingLevel>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));

	for (int i = this->vehicle->GetRaceComponent()->GetPosition() - 2; i >= 0; i--)
	{
		AVehiclePawn* otherVehicle = Cast<AVehiclePawn>(level->GetVehicle(i)->GetOwner());
		if (otherVehicle)
		{
			otherVehicle->InstantiateParticle(particle);
			otherVehicle->InvertControls();
			tricked.Add(otherVehicle);
		}
	}
}

void ALokiTrick::SetUp()
{
	Super::SetUp();
}

void ALokiTrick::Stop()
{
	timer = 0;
	for (AVehiclePawn* otherVehicle : tricked)
	{
		otherVehicle->NormalControls();
	}

	Super::Stop();
	Destroy();
}
