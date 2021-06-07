// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "ObjectBase.h"
#include "ProtectObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AProtectObjectBase : public AObjectBase
{
	GENERATED_BODY()

protected:
	virtual void SetUp() override;
	UFUNCTION()
	virtual void OnOverlapBegin(AActor* ownerActor, AActor* otherActor);
};

USTRUCT()
struct TFG_SOURCECODE_API FProtectObjectStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString race;
	UPROPERTY()
	int32 id;
	UPROPERTY()
	int32 objectType;
	UPROPERTY()
	FString parentUsername;

	UPROPERTY()
	int32 otherObjectId;
	UPROPERTY()
	int32 otherObjectType;
	UPROPERTY()
	FString otherObjectUsername;
};
