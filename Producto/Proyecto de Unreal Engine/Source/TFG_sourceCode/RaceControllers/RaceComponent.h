// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFG_SourceCode/Levels/BaseLevelActor.h"

#include "RaceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFG_SOURCECODE_API URaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URaceComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void SetCheckpoint(int checkpoint);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int currentCheckPoint = -1;
	int expectedCheckpoint = 0;
	int currentLap;
	float currentLapTime;
	ABaseLevelActor* level;
};
