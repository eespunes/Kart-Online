// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "MainMenuWidget.h"
#include "HttpModule.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	//When the object is constructed, Get the HTTP module
	http = &FHttpModule::Get();
}

void UMainMenuWidget::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

bool UMainMenuWidget::Login(FString username, FString password, bool save)
{

	return false;
}

bool UMainMenuWidget::Logout()
{
	return false;
}

bool UMainMenuWidget::Register(FString username, FString password, FString email)
{
	return false;
}
