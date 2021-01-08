// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "CheckPoint.h"

#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);
}

void ACheckPoint::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(otherActor);
	if (vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Overlapped %s -> %f, %f, %f"), *vehiclePawn->GetActorLabel(),
		       ownerActor->GetActorLocation().X, ownerActor->GetActorLocation().Y, ownerActor->GetActorLocation().Z);
	}
}
