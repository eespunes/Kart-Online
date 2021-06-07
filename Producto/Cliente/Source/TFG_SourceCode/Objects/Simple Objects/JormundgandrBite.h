// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"
#include "JormundgandrBite.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AJormundgandrBite : public AObjectBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
protected:
	virtual void SetUp() override;
	virtual void Stop() override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> jormundgandr;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	int32 attackPosition;
};
USTRUCT()
struct TFG_SOURCECODE_API FJormundgandrStruct
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
	FString objectiveUsername1;
	UPROPERTY()
	FString objectiveUsername2;
	UPROPERTY()
	FString objectiveUsername3;
};
