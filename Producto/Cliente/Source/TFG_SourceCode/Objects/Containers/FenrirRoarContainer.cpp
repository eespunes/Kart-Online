// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "FenrirRoarContainer.h"

#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void AFenrirRoarContainer::UseObject()
{
	Super::UseObject();
	ARacingLevel* level = Cast<ARacingLevel>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));

	int32 currentPosition = vehicle->GetRaceComponent()->GetPosition() - 1;
	int32 vehicleInFrontPosition = currentPosition - 1;
	int32 vehicleBehindPosition = currentPosition + 1;
	if (vehicleInFrontPosition >= 0 && vehicleBehindPosition < level->GetNumberOfVehicles())
	{
		Cast<AVehiclePawn>(level->GetVehicleByPosition(vehicleInFrontPosition)->GetOwner())->SetCurrentObject(fenrirRoar);
		Cast<AVehiclePawn>(level->GetVehicleByPosition(vehicleBehindPosition)->GetOwner())->SetCurrentObject(fenrirRoar);
	}
	else
	{
		if (vehicleInFrontPosition >= 0 || vehicleBehindPosition < level->GetNumberOfVehicles())
		{
			if (vehicleBehindPosition >= level->GetNumberOfVehicles())
			{
				Cast<AVehiclePawn>(level->GetVehicleByPosition(vehicleInFrontPosition)->GetOwner())->
					SetCurrentObject(fenrirRoar);
			}
			else if (vehicleInFrontPosition < 0)
			{
				Cast<AVehiclePawn>(level->GetVehicleByPosition(vehicleBehindPosition)->GetOwner())->SetCurrentObject(fenrirRoar);
			}
		}
	}

	Stop();
}

void AFenrirRoarContainer::Stop()
{
	Super::Stop();

	Destroy();
}
