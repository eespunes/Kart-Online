// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "HttpModule.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	 UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

private:
	FHttpModule* http;
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UFUNCTION(BlueprintPure)
	bool Login(FString username, FString password, bool save);
	UFUNCTION(BlueprintPure)
	bool Logout();
	UFUNCTION(BlueprintPure)
	bool Register(FString username, FString password, FString email);
};
