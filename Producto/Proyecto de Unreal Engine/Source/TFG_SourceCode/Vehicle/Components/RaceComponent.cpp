// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "RaceComponent.h"


#include <iterator>
#include <string>


#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"

// Sets default values for this component's properties
URaceComponent::URaceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void URaceComponent::BeginPlay()
{
	Super::BeginPlay();

	level = Cast<ARacingLevel>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));
	if (level)
	{
		maxCheckpoints = level->GetNumberOfCheckpoints();
		maxLaps = level->GetNumberOfLaps();
		currentCheckPoint = level->GetCheckpoint(0);
	}

	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}


// Called every frame
void URaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CanRace())
	{
		currentLapTime += DeltaTime;
		generalLapTime += DeltaTime;

		if (currentCheckPoint->IsPlayerInPlane(GetOwner()->GetActorLocation()))
		{
			SetCheckpoint();
			UE_LOG(LogTemp, Warning, TEXT("%i of %i"), currentCheckPoint->GetPosition(), maxCheckpoints)
		}
	}
}

void URaceComponent::SetCheckpoint()
{
	if (currentCheckPoint->GetPosition() == (maxCheckpoints - 1))
	{
		currentLap++;
		if (bestLapTime == -1 || currentLapTime <= bestLapTime)
			bestLapTime = currentLapTime;
		currentLapTime = 0;
		if (gameInstance->IsMultiplayer() && currentLap == maxLaps)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "L_MainMenu");
		}
	}

	currentCheckPoint = level->GetCheckpoint((currentCheckPoint->GetPosition() + 1) % maxCheckpoints);
}

int URaceComponent::GetPosition() const
{
	return position;
}

ACheckPoint* URaceComponent::GetCurrentCheckPoint() const
{
	return currentCheckPoint;
}

void URaceComponent::SetPosition(int32 Position)
{
	position = Position;
}

int32 URaceComponent::GetCurrentLap() const
{
	return currentLap;
}

int32 URaceComponent::GetMaxLap() const
{
	return maxLaps;
}

float URaceComponent::GetDistance()
{
	ACheckPoint* checkPoint = GetCurrentCheckPoint();
	FVector checkpointLocation = checkPoint->GetActorLocation();
	FVector vehicleLocation = GetOwner()->GetActorLocation();
	float distance = FVector::Distance(checkpointLocation, vehicleLocation);

	FVector direction = checkpointLocation - vehicleLocation;
	direction.Normalize();
	float angle = FMath::Acos(FVector::DotProduct(GetOwner()->GetActorForwardVector(), direction));

	return FMath::Abs(distance * FMath::Cos(angle));
}

float URaceComponent::GetTimeValue()
{
	return generalLapTime;
}

bool URaceComponent::InFrontOfOpponent(URaceComponent* opponent)
{
	return GetCurrentLap() > opponent->GetCurrentLap() ||
		GetCurrentLap() == opponent->GetCurrentLap() &&
		(GetExpectedCheckpoint() > opponent->GetExpectedCheckpoint() ||
			GetExpectedCheckpoint() == opponent->GetExpectedCheckpoint() && GetDistance() < opponent->GetDistance());
}

FString URaceComponent::GenerateTimeString(float time)
{
	if (time >= 0)
	{
		int32 minutes = time / 60;
		int32 seconds = time;
		seconds %= 60;
		int32 milliseconds = (time - ((minutes * 60) + seconds)) * 1000;

		FString minutesString = minutes < 10
			                        ? FString::Printf(TEXT("0%i"), minutes)
			                        : FString::Printf(TEXT("%i"), minutes);
		FString secondsString = seconds < 10
			                        ? FString::Printf(TEXT("0%i"), seconds)
			                        : FString::Printf(TEXT("%i"), seconds);
		FString millisecondsString = milliseconds < 10
			                             ? FString::Printf(TEXT("00%i"), milliseconds)
			                             : milliseconds < 100
			                             ? FString::Printf(TEXT("0%i"), milliseconds)
			                             : FString::Printf(TEXT("%i"), milliseconds);
		return FString::Printf(TEXT("%s:%s:%s"), *minutesString, *secondsString, *millisecondsString);
	}
	return TEXT("NO TIME");
}

float URaceComponent::GetCurrentLapTime() const
{
	return currentLapTime;
}

float URaceComponent::GetBestLapTime() const
{
	return bestLapTime;
}

bool URaceComponent::CanRace() const
{
	if (!level)
		return false;
	return level->GetCanRace();
}

void URaceComponent::SetCanRace(bool value) const
{
	if (!level)
		return;
	level->SetCanRace(value);
}

bool URaceComponent::CanStartRace() const
{
	if (!level)
		return false;
	return level->GetCanStartRace();
}

// void URaceComponent::CalculateTimeValue()
// {
// 	timeValue = FMath::Pow(currentLap + 1, currentLap + 1) *  (currentCheckPoint->GetPosition() + 1);
// }


void URaceComponent::Ragnarok(int racePosition)
{
	currentCheckPoint = level->GetCheckpoint(0);
	currentLap = maxLaps - 1;
	racePosition = (level->GetNumberOfVehicles() - 1) - racePosition;
	GetOwner()->SetActorLocation(level->GetRaceStart(racePosition)->GetActorLocation());
	GetOwner()->SetActorRotation(level->GetRaceStart(racePosition)->GetActorRotation());
}

int32 URaceComponent::GetExpectedCheckpoint() const
{
	return currentCheckPoint->GetPosition();
}
