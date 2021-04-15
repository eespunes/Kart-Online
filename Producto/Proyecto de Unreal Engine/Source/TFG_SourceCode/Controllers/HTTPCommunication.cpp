// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "HTTPCommunication.h"


#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "TFG_SourceCode/Widgets/MainMenuWidget.h"

void UHTTPCommunication::SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json)
{
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
