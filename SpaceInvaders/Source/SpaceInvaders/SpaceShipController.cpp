// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "SpaceShipController.h"


// Sets default values for this component's properties
USpaceShipController::USpaceShipController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpaceShipController::BeginPlay()
{
	Super::BeginPlay();

	// Set up KeyBindings
	UInputComponent* InputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find the input controller!?"));
	}
	else {
		InputComponent->BindAction(FName("Left"), IE_Pressed, this, &USpaceShipController::MoveLeft);
		InputComponent->BindAction(FName("Left"), IE_Released, this, &USpaceShipController::StopMoveLeft);

		InputComponent->BindAction(FName("Right"), IE_Pressed, this, &USpaceShipController::MoveRight);
		InputComponent->BindAction(FName("Right"), IE_Released, this, &USpaceShipController::StopMoveRight);

		InputComponent->BindAction(FName("Fire"), IE_Pressed, this, &USpaceShipController::Fire);
	}
}

void USpaceShipController::MoveLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Should be moving Left"));
	MovingLeft = true;
}

void USpaceShipController::StopMoveLeft()
{
	MovingLeft = false;
}

void USpaceShipController::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Should be moving Right"));
	MovingRight = true;
}

void USpaceShipController::StopMoveRight()
{
	MovingRight = false;
}

void USpaceShipController::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Should be moving Firing"));
}

// Called every frame
void USpaceShipController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (MovingLeft && MovingRight)
	{
		// Do nothing here, because the movements cancel eachother out...
		return;
	}

	if (MovingLeft) {
		FTransform ShipTransform = GetOwner()->GetTransform();
		FVector ShipPosition = ShipTransform.GetLocation();

		// Add to the X axis...
		ShipPosition.X += 50;
		ShipTransform.SetLocation(ShipPosition);
		GetOwner()->SetActorTransform(ShipTransform, false, nullptr, ETeleportType::None);
		return;
	}

	if (MovingRight)
	{
		FTransform ShipTransform = GetOwner()->GetTransform();
		FVector ShipPosition = ShipTransform.GetLocation();

		// Add to the X axis...
		ShipPosition.X -= 50;
		ShipTransform.SetLocation(ShipPosition);
		GetOwner()->SetActorTransform(ShipTransform, false, nullptr, ETeleportType::None);
		return;
	}
}

