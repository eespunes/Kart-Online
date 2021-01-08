// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

UCLASS()
class TFG_SOURCECODE_API ACheckPoint : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
    void OnOverlapBegin(AActor* ownerActor, AActor* otherActor);
};
