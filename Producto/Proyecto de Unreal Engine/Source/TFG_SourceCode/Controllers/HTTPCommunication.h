// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "SocketIOClient.h"
#include "SocketIONative.h"
#include "UObject/NoExportTypes.h"
#include "HTTPCommunication.generated.h"

class IHttpRequest;
class FHttpModule;
class USocketIOClientComponent;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API UHTTPCommunication : public UObject
{
	GENERATED_BODY()
public:
	UHTTPCommunication();
	void SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json);
	bool IsSuccessful(int32 responseCode);
	void SendGet(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, const FString& uri);

	void ConnectSocket();
	void ReceiveEvents();
	void SetRaceID(const FString&  id);
private:
	const FString URL = "http://localhost:3000/";
	FHttpModule* http;
	TSharedPtr<FSocketIONative> socketComponent;
	FString raceID;
};
