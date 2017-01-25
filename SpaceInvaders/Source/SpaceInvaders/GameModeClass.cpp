// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "GameModeClass.h"
#include "Explosion.h"
#include "DateTime.h"

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

	static ConstructorHelpers::FObjectFinder<USoundWave> AlienDieSndWave(TEXT("SoundWave'/Game/Sounds/invaderkilled.invaderkilled'"));
	AlienDieSound = AlienDieSndWave.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> AlienMoveSndWave1(TEXT("SoundWave'/Game/Sounds/fastinvader1.fastinvader1'"));
	AlienMoveSound1 = AlienMoveSndWave1.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> AlienMoveSndWave2(TEXT("SoundWave'/Game/Sounds/fastinvader2.fastinvader2'"));
	AlienMoveSound2 = AlienMoveSndWave2.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> AlienMoveSndWave3(TEXT("SoundWave'/Game/Sounds/fastinvader3.fastinvader3'"));
	AlienMoveSound3 = AlienMoveSndWave3.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> AlienMoveSndWave4(TEXT("SoundWave'/Game/Sounds/fastinvader4.fastinvader4'"));
	AlienMoveSound4 = AlienMoveSndWave4.Object;
}

void AGameModeClass::BeginPlay()
{
	Super::BeginPlay();

	SpawnAllShips();

	ourHUD = Cast<ASpaceInvadersHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	FDateTime Now = FDateTime::Now();

	int64 time = Now.ToUnixTimestamp();

	UE_LOG(LogTemp, Error, TEXT("Time: %s"), *FString::FromInt(time));

	FMath::SRandInit(time);
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

	if (ourHUD->GameOver)
		return;

	if (AlienArray.Num() == 0)
	{
		NextLevel = true;
	}

	if (NextLevel)
	{
		DestroyAllShips();
		SpawnAllShips();
		NextLevel = false;
		SecondsPerStep = 1.0f;
		StepCounter = 0.f;
		TotalSteps = 0;
		Steps = 6;
		Direction = true;
	}

	float randSpawn = FMath::RandRange(0, 100000);

	if (randSpawn > 99990)
	{
		SpawnMysteryShip();
	}

	for (AAlienShipPreset* Alien : AlienArray)
	{
		if (Alien->needDelete)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), AlienDieSound, 1.f, 1.f, 0.f, nullptr);
			CreateExplosionParticleEffect(Alien->GetActorTransform());
			RemoveActor(Alien);
			Alien->Destroy();
			break;
		}
	}

	for (AAlienShipPreset* MShip : MysteryArray)
	{
		if (MShip->needDelete)
		{
			if (MShip->DeathByPlayer)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), AlienDieSound, 1.f, 1.f, 0.f, nullptr);
				FTransform ft = MShip->GetActorTransform();
				CreateExplosionParticleEffect(ft);
			}
			MysteryArray.Remove(MShip);
			MShip->Destroy();
			break;
		}
	}

	if (StepCounter >= SecondsPerStep)
	{
		AAlienShipPreset* LeftAlien = GetMostLeftAlien();
		AAlienShipPreset* RightAlien = GetMostRightAlien();
		if (!LeftAlien)
			return;
		if (!RightAlien)
			return;

		TotalSteps++;
		StepCounter = 0.f;
		//UE_LOG(LogTemp, Warning, TEXT("STEP: %s"), *FString::SanitizeFloat(SecondsPerStep));
		SecondsPerStep = 1.f - (1.f / (0.1*AlienArray.Num() + 1.f));
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(SecondsPerStep));

		if (Direction)
		{
			PlayRandomMoveSound();
			float CurX = LeftAlien->GetActorLocation().X;
			if (CurX + 500.f < 10000.f) {
				MoveAllActors(FVector(500.f, 0.f, 0.f));
			}
			else {
					MoveAllActors(FVector(0.f, -500.f, 0.f));
					Direction = false;
			}
		}
		else {
			PlayRandomMoveSound();
			float CurX = RightAlien->GetActorLocation().X;

			if (CurX + 500.f > -10000.f)
			{
				MoveAllActors(FVector(-500.f, 0.f, 0.f));
			}
			else {
					MoveAllActors(FVector(0.f, -500.f, 0.f));
					Direction = true;
			}
		}
	}

	if ((ourHUD->GetScore() - LastLevelScore) > RequiredPoints && !bonusRecieved)
	{
		ourHUD->SetLives(ourHUD->GetLives() + 1);
		LastLevelScore = ourHUD->GetScore();
		SetRequirement();
	}

	StepCounter += DeltaTime;
}

void AGameModeClass::SetRequirement()
{
	int rand = FMath::RandRange(0.0f, 10.0f);

	if (rand > 5)
	{
		// 1500 points for new life
		RequiredPoints = 1500;
	}
	else if (rand < 5)
	{
		// 1000 points for new life
		RequiredPoints = 1000;
	}
}

void AGameModeClass::CreateExplosionParticleEffect(FTransform t)
{
	t.SetScale3D(FVector(0.4f, 0.4f, 0.4f));
	auto MyDeferredActor = Cast<AExplosion>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AExplosion::StaticClass(), t));

	if (MyDeferredActor)
	{
		UGameplayStatics::FinishSpawningActor(MyDeferredActor, t);
	}
}

void AGameModeClass::SpawnMysteryShip()
{
	FTransform t = FTransform();
	t.SetScale3D(FVector(4.f, 4.f, 4.f));

	t.SetLocation(FVector(-10000.f, 11500.f, 0.f));

	auto MyDeferredActor = Cast<AAlienShipPreset>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AAlienShipPreset::StaticClass(), t));

	// Randomly select between three things...

	float rand = FMath::RandRange(0, 100);

	int PointValue = 0;

	if (rand < 25 && rand >= 0)
	{
		PointValue = 50;
	}
	else if (rand >= 25 && rand < 50)
	{
		PointValue = 100;
	}
	else if (rand >= 50 && rand < 75)
	{
		PointValue = 150;
	}
	else if (rand >= 75 && rand <= 100)
	{
		PointValue = 300;
	}


	if (MyDeferredActor)
	{
		MyDeferredActor->SetMeshNum(3);
		MyDeferredActor->SetValue(PointValue);

		UGameplayStatics::FinishSpawningActor(MyDeferredActor, t);
		MysteryArray.Add(MyDeferredActor);
	}
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
			ourHUD->SetGameOver(true);
		}
	}

	for (AAlienShipPreset* Alien : AlienArray)
	{
		Alien->SetCanCheckRayCast(true);
	}
}

void AGameModeClass::DestroyAllShips()
{
	for (AAlienShipPreset* Alien : AlienArray)
	{
		Alien->Destroy();
	}

	for (AAlienShipPreset* MShip : MysteryArray)
	{
		MShip->Destroy();
	}

	AlienArray.Empty();
	MysteryArray.Empty();
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
			SpawnActor(0, 40, t);
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

void AGameModeClass::RestartGame()
{
	DestroyAllShips();
	SpawnAllShips();
	NextLevel = false;
	ourHUD->SetGameOver(false);
	ourHUD->SetScore(0);
	ourHUD->SetLives(2);
	SecondsPerStep = 1.0f;
	StepCounter = 0.f;
	TotalSteps = 0;
	Steps = 6;
	Direction = true;
	
	SetRequirement();
	
}

AAlienShipPreset* AGameModeClass::GetMostLeftAlien()
{
	// Highest X value
	float MaxX = -INT_MAX;
	float CurX = 0.f;

	AAlienShipPreset* MostLeftAlien = nullptr;

	for (AAlienShipPreset* Alien : AlienArray)
	{
		if (Alien)
		{
			CurX = Alien->GetActorLocation().X;

			if (CurX > MaxX)
			{
				MaxX = CurX;
				MostLeftAlien = Alien;
			}
		}
	}

	return MostLeftAlien;
}

AAlienShipPreset* AGameModeClass::GetMostRightAlien()
{
	// Lowest X value
	float MinX = INT_MAX;
	float CurX = 0.f;

	AAlienShipPreset* MostRightAlien = nullptr;

	for (AAlienShipPreset* Alien : AlienArray)
	{
		if (Alien)
		{
			CurX = Alien->GetActorLocation().X;

			if (CurX < MinX)
			{
				MinX = CurX;
				MostRightAlien = Alien;
			}
		}
	}

	return MostRightAlien;
}

void AGameModeClass::PlayRandomMoveSound()
{
	float Random = FMath::RandRange(0.f, 1000.f);

	if(Random <= 250)
		UGameplayStatics::PlaySound2D(GetWorld(), AlienMoveSound1, 1.f, 1.f, 0.f, nullptr);
	if(Random > 250 && Random <= 500)
		UGameplayStatics::PlaySound2D(GetWorld(), AlienMoveSound2, 1.f, 1.f, 0.f, nullptr);
	if(Random > 500 && Random <= 750)
		UGameplayStatics::PlaySound2D(GetWorld(), AlienMoveSound3, 1.f, 1.f, 0.f, nullptr);
	if(Random > 750 && Random <= 1000)
		UGameplayStatics::PlaySound2D(GetWorld(), AlienMoveSound1, 1.f, 1.f, 0.f, nullptr);
}