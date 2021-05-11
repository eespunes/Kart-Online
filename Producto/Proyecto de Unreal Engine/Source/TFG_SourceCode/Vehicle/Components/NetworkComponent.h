// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "NetworkComponent.generated.h"


class URaceGameInstance;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TFG_SOURCECODE_API UNetworkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNetworkComponent();

protected:
	void SendMessage(FString message);
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	URaceGameInstance* gameInstance;
	FString raceID;
	FString username;
	float sendTime = 0.250f;
	float timer = 0;
};

USTRUCT()
struct TFG_SOURCECODE_API FRacingStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString race;
	UPROPERTY()
	FString username;

	UPROPERTY()
	FString message;
};
