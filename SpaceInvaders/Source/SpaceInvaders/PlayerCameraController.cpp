// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "PlayerCameraController.h"


// Sets default values for this component's properties
UPlayerCameraController::UPlayerCameraController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCameraController::BeginPlay()
{
	Super::BeginPlay();

	// Sett kameraet opp sånn at spilleren ser gjennom det
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FViewTargetTransitionParams Params;
	Params.BlendExp = 0.f;
	Params.BlendFunction = VTBlend_Linear;
	Params.bLockOutgoing = 0;
	Params.BlendTime = 0.f;

	PlayerController->SetViewTarget(GetOwner(), Params);
}

// Called every frame
void UPlayerCameraController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

