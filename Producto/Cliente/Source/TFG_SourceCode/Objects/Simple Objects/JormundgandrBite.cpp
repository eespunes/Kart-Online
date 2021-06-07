// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "JormundgandrBite.h"


#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/Objects/Base/ProtectObjectBase.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void AJormundgandrBite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJormundgandrBite::UseObject()
{
	Super::UseObject();
	mesh->DestroyComponent();

	ARacingLevel* level = Cast<ARacingLevel>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));

	int32 currentPosition = vehicle->GetRaceComponent()->GetPosition() - 1;

	TArray<AVehiclePawn*> vehicles;
	for (int i = 0; i < attackPosition && i < currentPosition; i++)
	{
		vehicles.Add(Cast<AVehiclePawn>(level->GetVehicleByPosition(i)->GetOwner()));
	}
	for (AVehiclePawn* otherVehicle : vehicles)
	{
		otherVehicle->InstantiateParticle(jormundgandr);
		if (!Cast<AProtectObjectBase>(otherVehicle->GetCurrentObject()))
			otherVehicle->Damage();
	}

	Stop();
	Destroy();
}

void AJormundgandrBite::SetUp()
{
	Super::SetUp();
}

void AJormundgandrBite::Stop()
{
	Super::Stop();
	Destroy();
}
