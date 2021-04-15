// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HTTPCommunication.generated.h"

class IHttpRequest;
class FHttpModule;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API UHTTPCommunication : public UObject
{
	GENERATED_BODY()
public:
	void SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json);
	bool IsSuccessful(int32 responseCode);
	void SendGet(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, const FString& uri);
private:
	const FString URL = "https://tfg-videojocs.herokuapp.com";
	FHttpModule* http;
};
