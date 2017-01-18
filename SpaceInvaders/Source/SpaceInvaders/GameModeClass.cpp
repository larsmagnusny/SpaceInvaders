// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "GameModeClass.h"
#include "SpaceInvadersHUD.h"

AGameModeClass::AGameModeClass()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Blueprint Pawn...
	static ConstructorHelpers::FObjectFinder<UBlueprint> Blueprint(TEXT("Blueprint'/Game/DefaultPawn_Blueprint.DefaultPawn_Blueprint'"));
	if (Blueprint.Object)
	{
		DefaultPawnClass = (UClass*)Blueprint.Object->GeneratedClass;
	}

	HUDClass = ASpaceInvadersHUD::StaticClass();
}

void AGameModeClass::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetFirstPlayerController()->ClientSetHUD(HUDClass);

	SpawnAllShips();

	// Tell the playercontroller the HUD has changed...
}

void AGameModeClass::SpawnActor(int32 Type, int32 Points, FTransform t)
{
	auto MyDeferredActor = Cast<AAlienShipPreset>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AAlienShipPreset::StaticClass(), t));

	if (MyDeferredActor)
	{
		MyDeferredActor->SetMeshNum(Type);
		MyDeferredActor->SetValue(Points);

		UGameplayStatics::FinishSpawningActor(MyDeferredActor, t);
		AlienArray.Add(MyDeferredActor);
	}
}

void AGameModeClass::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor( DeltaTime, TickType, ThisTickFunction );

	if (AlienArray.Num() == 0)
	{
		GameOver = true;
	}

	if (GameOver)
	{
		DestroyAllShips();
		SpawnAllShips();
		GameOver = false;
		SecondsPerStep = 1.0f;
		StepCounter = 0.f;
		TotalSteps = 0;
		Steps = 6;
		Direction = true;
	}

	for (AAlienShipPreset* Alien : AlienArray)
	{
		if (Alien->needDelete)
		{
			RemoveActor(Alien);
			Alien->Destroy();
			return;
		}
	}

	if (StepCounter >= SecondsPerStep)
	{
		TotalSteps++;
		StepCounter = 0.f;

		//UE_LOG(LogTemp, Warning, TEXT("STEP: %s"), *FString::SanitizeFloat(SecondsPerStep));
		if (SecondsPerStep > 0.15)
		{
			SecondsPerStep -= 0.005f;
		}
		else {
			SecondsPerStep = 0.15;
		}

		if (Direction)
		{
			if (Steps > 0) {
				Steps--;
				MoveAllActors(FVector(500.f, 0.f, 0.f));
			}
			else {
					DownSteps--;
					MoveAllActors(FVector(0.f, -500.f, 0.f));
					Steps = 12;
					Direction = false;
			}
		}
		else {
			if (Steps > 0)
			{
				Steps--;
				MoveAllActors(FVector(-500.f, 0.f, 0.f));
			}
			else {
					MoveAllActors(FVector(0.f, -500.f, 0.f));
					Steps = 12;
					Direction = true;
			}
		}
	}

	StepCounter += DeltaTime;
}

void AGameModeClass::RemoveActor(AAlienShipPreset* ActorToDelete)
{
	for (int32 i = 0; i < AlienArray.Num(); ++i)
	{
		if (AlienArray[i] == ActorToDelete)
		{
			AlienArray.RemoveAt(i);
			break;
		}
	}
}

void AGameModeClass::MoveAllActors(FVector Step)
{
	for (AAlienShipPreset* Alien : AlienArray)
	{
		FVector Location = Alien->GetActorLocation() + Step;
		Alien->SetActorLocation(Location);
		if (Location.Y < 0)
		{
			// Trigger Game Over...
			UE_LOG(LogTemp, Warning, TEXT("Should trigger GameOver here!"));
			GameOver = true;
		}
	}
}

void AGameModeClass::DestroyAllShips()
{
	for (AAlienShipPreset* Alien : AlienArray)
	{
		Alien->Destroy();
	}

	AlienArray.Empty();
}

void AGameModeClass::SpawnAllShips()
{
	UWorld* const World = GetWorld();

	float StartX = 7200.0f;
	float StartY = 11000.0f;

	if (World)
	{
		for (int32 i = 0; i < 12; ++i) {
			FTransform t = FTransform();
			t.SetLocation(FVector(StartX - (1300.f*i), StartY, 0.f));
			t.SetScale3D(FVector(8.f, 8.f, 8.f));
			SpawnActor(0, 30, t);
		}

		for (int32 i = 1; i < 3; ++i)
		{
			for (int32 j = 0; j < 12; ++j) {
				FTransform t = FTransform();
				t.SetLocation(FVector(StartX - (1300.f*j), StartY - (1300.f*i), 0.f));
				t.SetScale3D(FVector(13.f, 13.f, 13.f));
				t.SetRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.0f, 90.0f)));
				SpawnActor(1, 20, t);
			}
		}

		for (int32 i = 3; i < 5; ++i)
		{
			for (int32 j = 0; j < 12; ++j) {
				FTransform t = FTransform();
				t.SetLocation(FVector(StartX - (1300.f*j), StartY - (1300.f*i), 0.f));
				t.SetScale3D(FVector(15.f, 15.f, 15.f));
				t.SetRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.0f, 90.0f)));
				SpawnActor(2, 10, t);
			}
		}
	}
}