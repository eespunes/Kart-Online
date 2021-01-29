// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFG_SourceCode/Levels/BaseLevelActor.h"

#include "RaceComponent.generated.h"

class ACheckPoint;
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
	UFUNCTION(BlueprintPure)
	int32 GetPosition() const;
	void CalculateDistance(ACheckPoint* checkpoint);
	int32 GetExpectedCheckpoint() const;
	void SetPosition(int32 Position);

	UFUNCTION(BlueprintPure)
	int32 GetCurrentLap() const;
	UFUNCTION(BlueprintPure)
	int32 GetMaxLap() const;
	UFUNCTION(BlueprintPure)
	FString GenerateTimeString(float time) const;
	UFUNCTION(BlueprintPure)
	float GetCurrentLapTime() const;
	UFUNCTION(BlueprintPure)
	float GetBestLapTime() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int32 maxCheckpoints;
	int32 currentCheckPoint = -1;
	int32 expectedCheckpoint = 0;
	int32 maxLaps;
	int32 currentLap;
	int32 position = -1;
	float currentLapTime;
	float bestLapTime = -1;
	float distance;
};
