// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "MainMenuWidget.h"

#include <string>


#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "TFG_SourceCode/Controllers/HTTPCommunication.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//When the object is constructed, Get the HTTP module
	http = &FHttpModule::Get();
	controller = NewObject<UHTTPCommunication>();
}

bool UMainMenuWidget::GetResponseRecieved() const
{
	return responseRecieved;
}

bool UMainMenuWidget::GetResponseCorrect() const
{
	return responseCorrect;
}

void UMainMenuWidget::Login(FString username, FString password, bool save)
{
	responseCorrect = false;
	responseRecieved = false;

	FString json;
	FLoginStruct loginStruct;
	loginStruct.username = username;
	loginStruct.password = password;
	FJsonObjectConverter::UStructToJsonObjectString(FLoginStruct::StaticStruct(), &loginStruct, json, 0, 0);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseReceived);

	controller->SendPost(Request, "api/login", json);
}

void UMainMenuWidget::Logout()
{
	responseCorrect = false;
	responseRecieved = false;

	FString json;
	FLogoutStruct logoutStruct;
	logoutStruct.username = GetUsername();
	FJsonObjectConverter::UStructToJsonObjectString(FLogoutStruct::StaticStruct(), &logoutStruct, json, 0, 0);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseReceived);

	controller->SendPost(Request, "api/logout", json);
}

void UMainMenuWidget::DeletePlayerInfo()
{
	playerLoggedIn = nullptr;
}

TArray<FString> UMainMenuWidget::GetPlayersMatchMaking()
{
	return TArray<FString>{TEXT("MANU"),TEXT("JAIME"),TEXT("CHRIS"),TEXT("FRANK"),TEXT("MANU"),TEXT("JAIME"),TEXT("CHRIS"),TEXT("FRANK")};
}

void UMainMenuWidget::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (controller->IsSuccessful(Response->GetResponseCode()))
	{
		responseCorrect = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%i"), Response->GetResponseCode())
	}
	responseRecieved = true;
}

void UMainMenuWidget::Register(FString username, FString password, FString email)
{
	responseCorrect = false;
	responseRecieved = false;

	FString json;
	FRegisterStruct registerStruct;
	registerStruct.username = username;
	registerStruct.password = password;
	registerStruct.email = email;
	FJsonObjectConverter::UStructToJsonObjectString(FRegisterStruct::StaticStruct(), &registerStruct, json, 0, 0);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseReceived);

	controller->SendPost(Request, "api/player", json);
}

void UMainMenuWidget::OnResponseUserReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (controller->IsSuccessful(Response->GetResponseCode()))
	{
		responseCorrect = true;
		TArray<FPlayerStruct> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(Response->GetContentAsString(), &data, 0, 0);

		if (data.Num() > 0)
		{
			playerLoggedIn = new FPlayerStruct(&data[0]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Response Code: %i"), Response->GetResponseCode())
	}
	responseRecieved = true;
}

void UMainMenuWidget::FindUserData(FString username)
{
	responseCorrect = false;
	responseRecieved = false;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseUserReceived);
	UE_LOG(LogTemp, Error, TEXT("Username: %s"), *username)
	controller->SendGet(Request, "api/player/" + username);
}

FString UMainMenuWidget::GetUsername() const
{
	if (!playerLoggedIn)
		return "";
	return playerLoggedIn->pla_username;
}

FString UMainMenuWidget::GetPassword() const
{
	if (!playerLoggedIn)
		return "";
	return playerLoggedIn->pla_password;
}

FString UMainMenuWidget::GetEmail() const
{
	if (!playerLoggedIn)
		return "";
	return playerLoggedIn->pla_email;
}

int32 UMainMenuWidget::GetRegistrationDate() const
{
	if (!playerLoggedIn)
		return -1;
	// return playerLoggedIn->registrationDate;
	return 2021;
}

int32 UMainMenuWidget::GetLevel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_level;
}

int32 UMainMenuWidget::GetExperiencePoints() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_experiencepoints;
}

int32 UMainMenuWidget::GetGoldenSteeringWheel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_goldensteeringwheel;
}

int32 UMainMenuWidget::GetSilverSteeringWheel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_silversteeringwheel;
}

int32 UMainMenuWidget::GetBronzeSteeringWheel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_bronzesteeringwheel;
}

int32 UMainMenuWidget::GetWoodenSteeringWheel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_woodensteeringwheel;
}

bool UMainMenuWidget::PlayerIsLoggedIn() const
{
	if (!playerLoggedIn)
		return false;
	return true;
}
