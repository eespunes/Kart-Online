// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "HTTPCommunication.h"


#include "HttpModule.h"
#include "SocketIOClient.h"
#include "Interfaces/IHttpRequest.h"
#include "TFG_SourceCode/Widgets/MainMenuWidget.h"
#include "SocketIOClientComponent.h"

UHTTPCommunication::UHTTPCommunication()
{
	socketComponent = ISocketIOClientModule::Get().NewValidNativePointer();
	if (socketComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket created"));
	}
}

void UHTTPCommunication::SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json)
{
	UE_LOG(LogTemp, Error, TEXT("WTF? %s"), *(URL + uri));
	request->SetURL(URL + uri);
	request->SetVerb("POST");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->SetContentAsString(json);
	request->ProcessRequest();
}

bool UHTTPCommunication::IsSuccessful(int32 responseCode)
{
	return responseCode / 100 == 2;
}

void UHTTPCommunication::SendGet(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, const FString& uri)
{
	UE_LOG(LogTemp, Error, TEXT("URL: %s%s"), *URL, *uri)
	request->SetURL(URL + uri);
	request->SetVerb("GET");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->ProcessRequest();
}

void UHTTPCommunication::ConnectSocket()
{
	if (!socketComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Attempting Connection"))
	socketComponent->Connect(TEXT("http://localhost:3000"), nullptr, nullptr);
	FString connected = socketComponent->bIsConnected ? TEXT("True") : TEXT("False");
	UE_LOG(LogTemp, Warning, TEXT("Connection: %s"), *connected);

	// ReceiveEvents();
}

void UHTTPCommunication::ReceiveEvents()
{
	if (!socketComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}

	socketComponent->OnEvent(raceID, [](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		UE_LOG(LogTemp, Log, TEXT("Received: %s"), *USIOJConvert::ToJsonString(Message));
	});
}

void UHTTPCommunication::SetRaceID(const FString& id)
{
	raceID = id;
}
