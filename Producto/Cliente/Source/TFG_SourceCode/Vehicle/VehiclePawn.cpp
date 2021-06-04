// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

#include <valarray>


#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/NetworkComponent.h"
#include "Components/RaceComponent.h"
#include "Components/TyreComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"

// Sets default values
AVehiclePawn::AVehiclePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// RootComponent = root;

	carMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Mesh"));
	RootComponent = carMesh;

	objectSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Object Spawn Point"));
	objectSpawnPoint->SetupAttachment(carMesh);

	particleSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Particle Spawn Point"));
	particleSpawnPoint->SetupAttachment(carMesh);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(carMesh);

	normalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	normalCamera->SetupAttachment(springArm);
	normalCamera->Activate();

	reverseCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Reverse Camera"));
	reverseCamera->SetupAttachment(carMesh);
	reverseCamera->Deactivate();

	playerNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player Name"));
	playerNameText->SetupAttachment(carMesh);

	raceComponent = CreateDefaultSubobject<URaceComponent>(TEXT("Race Component"));
	networkComponent = CreateDefaultSubobject<UNetworkComponent>(TEXT("Network Component"));
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameInstance)
	{
		maxSpeed = gameInstance->GetDifficulty();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not found"))
	}

	lastUpVector = GetActorUpVector();

	reverseSpeed = -maxSpeed / reverseRate;
	// acceleration = maxSpeed;
	// reverseAcceleration = acceleration / reverseRate;
	initialMaxSpeed = maxSpeed;
	accelerationTimer = 1;
	brakeTimer = 1;
	decelerationTimer = 1;
	// decelerationTimer = 9999999;
}

// Called every frame
void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GravityForce();
	SuspensionForces();

	if (raceComponent->CanRace())
	{
		WaitAfterHit(DeltaTime);

		Movement();
	}
	else
	{
		carMesh->SetLinearDamping(1000000);
		carMesh->SetAngularDamping(1000000);
	}
}

/*
*########
*MOVEMENT
*########
*/

void AVehiclePawn::Movement()
{
	carMesh->SetLinearDamping(1);
	carMesh->SetAngularDamping(1);
	float currentAngular = (carMesh->GetPhysicsAngularVelocityInDegrees() * GetActorUpVector()).Size();
	if (inGround)
	{
		if (!invertControls && isAccelerating && !isBraking || invertControls && !isAccelerating && isBraking)
		{
			decelerationTimer = 1;

			currentSpeed = FMath::Min<float>(
				currentSpeed + maxSpeed * (FMath::Exp(accelerationTimer / accelerationRate) - 1) * GetWorld()->
				DeltaTimeSeconds, maxSpeed * terrainFriction);
			accelerationTimer += GetWorld()->DeltaTimeSeconds;

			// GetMesh()->SetRelativeRotation(FRotator(
			// 	FMath::Min<float>(
			// 		maxPitchRotation * (FMath::Abs(currentSpeed) / maxSpeed), maxPitchRotation), 0, 0));
		}
		else if (!invertControls && !isAccelerating && isBraking || invertControls && isAccelerating && !isBraking)
		{
			decelerationTimer = 1;

			currentSpeed = FMath::Max<float>(
				currentSpeed - maxSpeed * (FMath::Exp(brakeTimer / brakeRate) - 1) * GetWorld()->DeltaTimeSeconds,
				reverseSpeed);
			brakeTimer += GetWorld()->DeltaTimeSeconds;

			// GetMesh()->SetRelativeRotation(FRotator(
			// 	FMath::Max<float>(
			// 		-maxPitchRotation * (FMath::Abs(currentSpeed) / maxSpeed), -maxPitchRotation),
			// 	0, 0));
		}
		else
		{
			float exponentialValue = (FMath::Exp(decelerationTimer / decelerationRate) - 1);
			if (currentSpeed > 0)
			{
				currentSpeed = FMath::Max<float>(
					currentSpeed - maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);

				// GetMesh()->SetRelativeRotation(FRotator(
				// 	FMath::Max<float>(maxPitchRotation * (FMath::Abs(currentSpeed) / maxSpeed), 0), 0, 0));
			}
			else if (currentSpeed < 0)
			{
				currentSpeed = FMath::Min<float>(
					currentSpeed + maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);

				// GetMesh()->SetRelativeRotation(FRotator(
				// 	FMath::Min<float>(
				// 		-maxPitchRotation * (FMath::Abs(currentSpeed) / maxSpeed),
				// 		0), 0, 0));
			}
			decelerationTimer += GetWorld()->DeltaTimeSeconds;
		}


		// if (isDrifting)
		// {
		// 	PerformDrift(currentSpeed, currentAngular);
		// }
		// else
		// {
		// 	PerformSteering(currentSpeed, currentAngular);
		// }

		// float rightFrictionForce = FVector::DotProduct(carMesh->GetPhysicsLinearVelocity(), GetActorRightVector());
		//
		// carMesh->AddForce(rightFrictionForce * GetActorRightVector() * -1 * frictionRate);
		//
		// DrawDebugLine(GetWorld(), GetActorLocation(),
		//               GetActorLocation() * rightFrictionForce * GetActorRightVector() * -1 * frictionRate,
		//               FColor::Green, false, -1, 0, 5);
		//
		// float frontFrictionForce = FVector::DotProduct(carMesh->GetPhysicsLinearVelocity(), GetActorForwardVector());
		//
		// carMesh->AddForce(frontFrictionForce * GetActorForwardVector() * -1 * frictionRate);
		//

		carMesh->SetPhysicsLinearVelocity(GetForward() * currentSpeed);
	}
	else
	{
		float exponentialValue = (FMath::Exp(decelerationTimer / decelerationRate) - 1);
		if (currentSpeed > 0)
			currentSpeed = FMath::Max<float>(
				currentSpeed - maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);
		else if (currentSpeed < 0)
			currentSpeed = FMath::Min<float>(
				currentSpeed + maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);

		// GetMesh()->AddRelativeRotation(FRotator(-5 * GetWorld()->DeltaTimeSeconds, 0, 0));
		//
		// carMesh->SetPhysicsLinearVelocity(GetForward() * currentSpeed);
	}
	//
	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Left"))
			tyre->RotateTyres(-currentSpeed);
		else
			tyre->RotateTyres(currentSpeed);
	}
	//
	if (currentSpeed < 0)
	{
		normalCamera->Deactivate();
		reverseCamera->Activate();
	}
	else
	{
		reverseCamera->Deactivate();
		normalCamera->Activate();
	}
	//
	// if (performDriftBoost)
	// {
	// 	if (driftBoostTimer > driftBoostTime)
	// 	{
	// 		performDriftBoost = false;
	// 		SetMaxSpeed(initialMaxSpeed);
	// 		driftBoostTimer = 0;
	// 	}
	// 	else
	// 	{
	// 		driftBoostTimer += GetWorld()->DeltaTimeSeconds;
	// 	}
	// }

	//DEBUG
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange,
		                                 FString::Printf(
			                                 TEXT("Angular: %f"), GetMesh()->GetRelativeRotation().Pitch));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Yellow,
		                                 FString::Printf(
			                                 TEXT("Speed: %f"), currentSpeed));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue,
		//                                  FString::Printf(
		// 	                                 TEXT("%s Position= (%f,%f,%f)"), *networkComponent->username,
		// 	                                 GetActorLocation().X, GetActorLocation().Y,
		// 	                                 GetActorLocation().Z));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Red,
		//                                  FString::Printf(
		// 	                                 TEXT("%s Rotation= (%f,%f,%f)"), *networkComponent->username,
		// 	                                 GetActorRotation().Pitch, GetActorRotation().Yaw,
		// 	                                 GetActorRotation().Roll));
	}
}

/*
*##########
*ACCELERATE
*##########
*/

void AVehiclePawn::Accelerate()
{
	networkComponent->SetDataIsAccelerating();
	isAccelerating = networkComponent->GetDataIsAccelerating();
	if (!isAccelerating)
		accelerationTimer = 1;
}

// void AVehiclePawn::PerformAcceleration()
// {
// 	carMesh->SetLinearDamping(1.f);
// 	if (currentSpeed < maxSpeed * terrainFriction)
// 	{
// 		carMesh->AddForceAtLocation(
// 			GetForward() * acceleration * FMath::Exp(accelerationTimer) * accelerationRate,
// 			GetCenterOfMass());
// 		lastVelocity = currentSpeed;
// 		accelerationTimer += GetWorld()->DeltaTimeSeconds;
// 	}
// }

/*
*#####
*BRAKE
*#####
*/

void AVehiclePawn::Brake()
{
	networkComponent->SetDataIsBraking();
	isBraking = networkComponent->GetDataIsBraking();
	if (!isBraking)
	{
		brakeTimer = 1;
		// reverseTimer = 0;
	}
}

//
// void AVehiclePawn::PerformBraking(float& currentVelocity)
// {
// 	carMesh->SetLinearDamping(1.f);
// 	currentVelocity = lastVelocity < currentVelocity ? -currentVelocity : currentVelocity;
// 	GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue,
// 	                                 FString::Printf(
// 		                                 TEXT("Object= %f"),
// 		                                 currentVelocity));
// 	if (currentVelocity < maxSpeed / 20 && currentVelocity > reverseSpeed)
// 	{
// 		carMesh->AddForceAtLocation(
// 			-GetForward() * acceleration * FMath::Exp(reverseTimer) * accelerationRate,
// 			GetCenterOfMass());
// 		lastVelocity = currentVelocity;
// 		brakeTimer = 0;
// 		reverseTimer += GetWorld()->DeltaTimeSeconds;
// 	}
// 	else
// 	{
// 		carMesh->AddForceAtLocation(-GetForward() * acceleration * FMath::Exp(brakeTimer) * brakeRate,
// 		                            GetCenterOfMass());
// 		lastVelocity = currentVelocity;
// 		reverseTimer = 0;
// 		brakeTimer += GetWorld()->DeltaTimeSeconds;
// 	}
// }

/*
*#####
*STEER
*#####
*/

void AVehiclePawn::Steer(float value)
{
	networkComponent->SetDataTurnValue(invertControls ? -value : value);
	steerValue = networkComponent->GetDataTurnValue();
}

// float AVehiclePawn::CalculateMaxSteerValue(float currentVelocity)
// {
// 	float x = currentVelocity / maxSpeed;
// 	float steeringAngle = maxSteerAngle;
// 	if (x < 0) steeringAngle *= FMath::Sin(-4 * x);
// 	else if (x < 0.25) steeringAngle *= FMath::Sin(1885 * x / 300);
// 	else steeringAngle *= FMath::Sin((x / 1.15) + 1.5);
// 	return steeringAngle;
// }
//
// void AVehiclePawn::PerformSteering(float currentVelocity, float currentAngular)
// {
// 	carMesh->SetAngularDamping(10.f);
// 	float steeringAngle = CalculateMaxSteerValue(currentVelocity);
//
// 	carMesh->SetPhysicsMaxAngularVelocityInDegrees(steeringAngle);
//
// 	carMesh->AddTorqueInDegrees(
// 		carMesh->GetUpVector() * steeringRate * steerValue * steeringAngle,
// 		NAME_None, true);
//
// 	for (UTyreComponent* tyre : tyres)
// 	{
// 		if (traction4x4 || tyre->GetName().Contains("Front"))
// 			tyre->Steer(steerValue);
// 	}
// }

/*
*#####
*DRIFT
*#####
*/
void AVehiclePawn::Drift()
{
	networkComponent->SetDataIsDrifting();
	isDrifting = networkComponent->GetDataIsDrifting();

	if (isDrifting)
	{
		StartDrift();
	}
	else
	{
		StopDrift();
	}
}

//
// void AVehiclePawn::PerformDrift(float currentVelocity, float currentAngular)
// {
// 	carMesh->SetAngularDamping(5.f);
// 	if (driftSign == 0 || FMath::Abs(currentVelocity) < maxSpeed / 1.75f)
// 	{
// 		StopDrift();
// 		return;
// 	}
// 	if (currentAngular >= maxDriftAngle / 2)
// 	{
// 		if (!canDriftBoost)
// 		{
// 			InstantiateDriftBoostParticles();
// 			canDriftBoost = true;
// 		}
// 	}
// 	carMesh->SetPhysicsMaxAngularVelocityInDegrees(CalculateMaxDriftValue(currentVelocity));
//
// 	carMesh->AddTorqueInDegrees(carMesh->GetUpVector() * steeringRate * driftSign, NAME_None, true);
//
// 	for (UTyreComponent* tyre : tyres)
// 	{
// 		if (tyre->GetName().Contains("Front"))
// 			tyre->Drift(steerValue);
// 	}
// }
//

void AVehiclePawn::StartDrift()
{
	driftSign = FMath::Sign(steerValue);
	driftCameraRotationValue = -driftSign * cameraRotation;
	springArm->AddLocalRotation(FRotator(0, driftCameraRotationValue, 0));
}

void AVehiclePawn::StopDrift()
{
	isDrifting = false;
	driftTimer = 0;
	driftInverseTimer = 0;
	springArm->AddLocalRotation(FRotator(0, -driftCameraRotationValue, 0));
	driftCameraRotationValue = 0;

	if (canDriftBoost)
	{
		canDriftBoost = false;

		for (AActor* particle : boostParticles)
		{
			particle->Destroy();
		}
		boostParticles.Empty();
		SetMaxSpeed(initialMaxSpeed * driftBoostRate);
		performDriftBoost = true;
	}
}

//
// float AVehiclePawn::CalculateMaxDriftValue(float currentVelocity)
// {
// 	float MaxSteerValue = CalculateMaxSteerValue(currentVelocity);
// 	if (driftValue == 0)
// 		driftValue = MaxSteerValue;
//
// 	if (steerValue == 0)
// 	{
// 		driftTimer = 0;
// 		driftInverseTimer = 0;
// 	}
//
// 	if (FMath::Sign(steerValue) == driftSign)
// 	{
// 		driftInverseTimer = 0;
// 		driftTimer += FMath::Abs(steerValue) * GetWorld()->DeltaTimeSeconds;
// 		driftValue += (FMath::Exp(driftTimer / driftRateIncrease) - 1);
// 	}
// 	else
// 	{
// 		driftTimer = 0;
// 		driftInverseTimer += FMath::Abs(steerValue) * GetWorld()->DeltaTimeSeconds;
// 		driftValue -= (FMath::Exp(driftInverseTimer / driftRateDecrease) - 1);
// 	}
//
// 	return driftValue >= maxDriftAngle
// 		       ? maxDriftAngle
// 		       : driftValue <= MaxSteerValue
// 		       ? MaxSteerValue
// 		       : driftSign * driftValue;
// }

/*
*#######
*PHYSICS
*#######
*/

void AVehiclePawn::GravityForce() const
{
	carMesh->AddForce(GetActorUpVector() * GetWorld()->GetGravityZ(), NAME_None, true);
}

void AVehiclePawn::SuspensionForces()
{
	inGround = true;
	terrainFriction = 1;
	for (UTyreComponent* tyre : tyres)
	{
		if (!tyre->SuspensionForce(suspensionDistance, suspensionRate, dampingRate, &terrainFriction))
			inGround = false;
	}
}

void AVehiclePawn::InstantiateDriftBoostParticles()
{
	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Rear"))
		{
			AActor* particle = GetWorld()->SpawnActor<AActor>(
				driftBoostParticle,
				tyre->GetRootPoint()->GetComponentLocation(),
				tyre->GetRootPoint()->GetComponentRotation()
			);
			particle->AttachToComponent(tyre->GetRootPoint(), FAttachmentTransformRules::KeepWorldTransform);
			boostParticles.Add(particle);
		}
	}
}

/*
*####### #######
*OBJECTS RELATED
*####### #######
*/

void AVehiclePawn::UseObject()
{
	if (currentObject && !hasBeenHit)
	{
		currentObject->UseObject();
		networkComponent->SetUseObjectData(true);
		networkComponent->SetObjectData(-1);
	}
}

void AVehiclePawn::RemoveObject()
{
	if (currentObject)
	{
		currentObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		currentObject->SetOwner(nullptr);
		currentObject = nullptr;
		networkComponent->SetUseObjectData(false);
		networkComponent->SetObjectData(-1);
	}
}

void AVehiclePawn::Damage()
{
	hasBeenHit = true;
	maxSpeed = 0;
}

void AVehiclePawn::WaitAfterHit(float DeltaTime)
{
	if (hasBeenHit)
	{
		if (hitTimer >= hitWaiting)
		{
			hasBeenHit = false;
			maxSpeed = initialMaxSpeed;
			hitTimer = 0;
		}
		hitTimer += DeltaTime;
	}
}

void AVehiclePawn::InstantiateParticle(const TSubclassOf<AActor>& particle)
{
	currentParticle = GetWorld()->SpawnActor<AActor>(
		particle,
		particleSpawnPoint->GetComponentLocation(),
		particleSpawnPoint->GetComponentRotation()
	);
	currentParticle->AttachToComponent(particleSpawnPoint, FAttachmentTransformRules::KeepWorldTransform);
}

void AVehiclePawn::InvertControls()
{
	invertControls = true;
}

void AVehiclePawn::NormalControls()
{
	currentParticle->Destroy();
	invertControls = false;
}

/*
 *####### ### #######
 *GETTERS AND SETTERS
 *####### ### #######
 */

FVector AVehiclePawn::GetForward() const
{
	// FHitResult hit;
	// FVector position = GetActorLocation();
	// FVector end = position - (GetActorUpVector() * 300);
	// FVector forward = GetActorForwardVector();
	//
	// GetWorld()->LineTraceSingleByChannel(hit, position, end, ECC_WorldStatic);
	//
	// if (hit.bBlockingHit)
	// {
	// 	forward = GetActorForwardVector() - hit.Normal * GetActorForwardVector();
	// }
	//
	// end = position + (forward * 3000);
	//
	// GetWorld()->LineTraceSingleByChannel(hit, position, end, ECC_WorldStatic);
	// if (hit.bBlockingHit)
	// {
	// 	DrawDebugLine(GetWorld(), hit.ImpactPoint, hit.ImpactPoint + (hit.ImpactNormal+ * 3000), FColor::Blue, true, 100, 0, 10);
	// 	end = hit.ImpactPoint;
	// }
	//
	// DrawDebugLine(GetWorld(), position, position + (forward * 3000), FColor::Blue, false, -1, 0, 10);
	TArray<FVector> rearArray, frontArray;

	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Rear"))
		{
			rearArray.Add(tyre->GetImpactPoint());
		}
		else
		{
			frontArray.Add(tyre->GetImpactPoint());
		}
	}
	FVector rearVector, frontVector;
	for (auto rear : rearArray)
	{
		rearVector += rear;
	}
	rearVector /= rearArray.Num();

	for (auto front : frontArray)
	{
		frontVector += front;
	}
	frontVector /= frontArray.Num();


	FVector forward = frontVector - rearVector;
	forward.Normalize();
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (forward * 3000), FColor::Blue, false, -1, 0,
	              10);

	return forward;
}

FVector AVehiclePawn::GetUp() const
{
	FHitResult hit;
	FVector position = GetActorLocation();
	FVector end = position - (GetActorUpVector() * 3000);
	FVector up = GetActorUpVector();

	GetWorld()->LineTraceSingleByChannel(hit, position, end, ECC_WorldStatic);

	if (hit.bBlockingHit)
	{
		up = hit.Normal;
	}

	return up;
}

UStaticMeshComponent* AVehiclePawn::GetMesh() const
{
	return carMesh;
}

URaceComponent* AVehiclePawn::GetRaceComponent() const
{
	return raceComponent;
}

UNetworkComponent* AVehiclePawn::GetNetworkComponent() const
{
	return networkComponent;
}

AObjectBase* AVehiclePawn::GetCurrentObject() const
{
	return currentObject;
}

void AVehiclePawn::SetCurrentObject(TSubclassOf<UObject> CurrentObject)
{
	if (this->currentObject)
	{
		return;
	}
	if (CurrentObject)
	{
		networkComponent->SetUseObjectData(false);
		this->currentObject = GetWorld()->SpawnActor<AObjectBase>(CurrentObject,
		                                                          objectSpawnPoint->GetComponentLocation(),
		                                                          GetActorRotation());
		this->currentObject->SetVehicle(this);
		this->currentObject->SetOwner(this);
		this->currentObject->SetActorLocation(objectSpawnPoint->GetComponentLocation());
		this->currentObject->AttachToComponent(objectSpawnPoint, FAttachmentTransformRules::KeepWorldTransform);
	}
}

float AVehiclePawn::GetMaxSpeed() const
{
	return maxSpeed;
}

void AVehiclePawn::SetMaxSpeed(float speed)
{
	maxSpeed = speed;
	UE_LOG(LogTemp, Warning, TEXT("Current MAX Speed: %f"), maxSpeed)
	acceleration = maxSpeed;
}

float AVehiclePawn::GetInitialMaxSpeed() const
{
	return initialMaxSpeed;
}

FVector AVehiclePawn::GetCenterOfMass() const
{
	return carMesh->GetCenterOfMass() - GetActorUpVector() * 10 +
		GetActorForwardVector() * 10;
}

bool AVehiclePawn::GetDrifting() const
{
	return isDrifting;
}

bool AVehiclePawn::GetBraking()
{
	return isBraking;
}

float AVehiclePawn::GetCurrentSpeed()
{
	return currentSpeed;
}

float AVehiclePawn::GetDriftSign()
{
	return driftSign;
}

UTextRenderComponent* AVehiclePawn::GetPlayerText()
{
	return playerNameText;
}

void AVehiclePawn::SetPing(int32 ping)
{
	return networkComponent->SetPing(ping);
}

int32 AVehiclePawn::GetPing()
{
	return networkComponent->GetPing();
}

bool AVehiclePawn::GetHasBeenHit() const
{
	return hasBeenHit;
}

bool AVehiclePawn::GetIsAccelerating() const
{
	return isAccelerating;
}

void AVehiclePawn::SetIsAccelerating(bool bIsAccelerating)
{
	isAccelerating = bIsAccelerating;
}

void AVehiclePawn::SetIsBraking(bool bIsBraking)
{
	isBraking = bIsBraking;
}

void AVehiclePawn::SetSteerValue(float bSteerValue)
{
	steerValue = bSteerValue;
}

void AVehiclePawn::SetIsDrifting(bool bIsDrifting)
{
	isDrifting = bIsDrifting;
}
