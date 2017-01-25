// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "DestructionChecker.h"


// Sets default values for this component's properties
UDestructionChecker::UDestructionChecker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDestructionChecker::BeginPlay()
{
	Super::BeginPlay();

	// ...
	DestructionComp = Cast<UDestructibleComponent>(GetOwner()->GetComponentByClass(UDestructibleComponent::StaticClass()));
}


// Called every frame
void UDestructionChecker::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );


}

