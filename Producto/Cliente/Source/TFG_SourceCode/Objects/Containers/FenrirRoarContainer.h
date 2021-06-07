// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "TFG_SourceCode/Objects/Base/ObjectBase.h"

#include "FenrirRoarContainer.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AFenrirRoarContainer : public AObjectBase
{
	GENERATED_BODY()
public:
	virtual void UseObject() override;
protected:
	virtual void Stop() override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AObjectBase> fenrirRoar;
};

USTRUCT()
struct TFG_SOURCECODE_API FFenrirStruct
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
	FString objectiveUsername;
};
