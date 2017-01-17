// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "GameModeClass.h"
#include "AlienShipPreset.h"
#include "AlienShip_Second.h"
#include "AlienShip_Third.h"

AGameModeClass::AGameModeClass()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameModeClass::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();

	float StartX = 6800.0f;
	float StartY = 8000.0f;

	if (World)
	{
		for (int32 i = 0; i < 16; ++i) {
			FTransform t = FTransform();
			t.SetLocation(FVector(StartX - (900.0f*i), StartY, 0.f));
			t.SetScale3D(FVector(5.f, 5.f, 5.f));
			AAlienShipPreset* AlienShip = World->SpawnActor<AAlienShipPreset>(AAlienShipPreset::StaticClass(), t);
			AlienArray.Add(AlienShip);
		}

		for (int32 i = 1; i < 3; ++i)
		{
			for (int32 j = 0; j < 16; ++j) {
				FTransform t = FTransform();
				t.SetLocation(FVector(StartX - (900.0f*j), StartY - (1000.0*i), 0.f));
				t.SetScale3D(FVector(9.f, 9.f, 9.f));
				t.SetRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.0f, 90.0f)));
				AlienArray.Add(World->SpawnActor<AAlienShip_Second>(AAlienShip_Second::StaticClass(), t));
			}
		}

		for (int32 i = 3; i < 5; ++i)
		{
			for (int32 j = 0; j < 16; ++j) {
				FTransform t = FTransform();
				t.SetLocation(FVector(StartX - (900.0f*j), StartY - (1000.0*i), 0.f));
				t.SetScale3D(FVector(10.f, 10.f, 10.f));
				t.SetRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.0f, 90.0f)));
				AlienArray.Add(World->SpawnActor<AAlienShip_Third>(AAlienShip_Third::StaticClass(), t));
			}
		}
	}
}

void AGameModeClass::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor( DeltaTime, TickType, ThisTickFunction );

	// Let's check if the actors have been deleted?
	for (AActor* Ent : AlienArray)
	{
		if (Ent->IsA<AAlienShipPreset>())
		{
			AAlienShipPreset* AlienP = (AAlienShipPreset*)Ent;

			if (AlienP->needDelete)
			{
				AlienArray.Remove(Ent);
				AlienP->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("Ship has been destroyed!"));
			}
		}
		if (Ent->IsA<AAlienShip_Second>())
		{
			AAlienShip_Second* AlienP = (AAlienShip_Second*)Ent;

			if (AlienP->needDelete)
			{
				AlienArray.Remove(Ent);
				AlienP->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("Ship has been destroyed!"));
			}
		}
		if (Ent->IsA<AAlienShip_Third>())
		{
			AAlienShip_Third* AlienP = (AAlienShip_Third*)Ent;

			if (AlienP->needDelete)
			{
				AlienArray.Remove(Ent);
				AlienP->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("Ship has been destroyed!"));
			}
		}
	}
}

void AGameModeClass::RemoveActor(AActor* ActorToDelete)
{
	AlienArray.Remove(ActorToDelete);
}