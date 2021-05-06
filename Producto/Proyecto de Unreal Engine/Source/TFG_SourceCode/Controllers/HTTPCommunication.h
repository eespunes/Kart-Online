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
	void ReceiveFindRaceEvents();
	void ReceiveRaceEvents();
	void SendEvents(FString room, FString json);
	bool SocketIsConnected();
	FString GetRaceID();
	bool RaceFound();
	FString Timer();
	TArray<FString> GetPlayersInRace();
	bool GetPlayersUpdated();
	void DisconnectSocket();
private:
	const FString URL = "http://localhost:3000/";
	FHttpModule* http;
	TSharedPtr<FSocketIONative> socket;
	FString raceID;
	bool raceFound;
	bool playersArrayUpdated;
	TArray<FString> playersInRace;
	FString timer;
};

USTRUCT()
struct TFG_SOURCECODE_API FRaceStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString race;
	UPROPERTY()
	int32 status;

	FRaceStruct()
	{
	};

	FRaceStruct(FRaceStruct* other)
	{
		this->race = other->race;
		this->status = other->status;
	};
};
