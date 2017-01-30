// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "GameModeClass.h"
#include "Explosion.h"		// Vi må vite hva en eksplosjon er
#include "DateTime.h"		// For å hente UNIX time for å seede random funksjonen

AGameModeClass::AGameModeClass()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Blueprint Pawn...

	DefaultPawnClass = nullptr;	// Not using a default pawn this gamemode...

	// Sett HUDClass
	HUDClass = ASpaceInvadersHUD::StaticClass();

	// Last inn lyder vi bruker
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

	// Spawn alle skipene
	SpawnAllShips();

	// hent en peker til vår hudclasse
	ourHUD = Cast<ASpaceInvadersHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Hent tiden nå
	FDateTime Now = FDateTime::Now();
	int64 time = Now.ToUnixTimestamp();

	// Seed random
	FMath::SRandInit(time);
}

void AGameModeClass::SpawnActor(int32 Type, int32 Points, FTransform t)
{
	// Deferred actor spawn så vi kan kjøre funksjoner vår konstructoren blir kjørt
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

	// Hvis vi ikke har noen alienSkip så kan vi kjøre neste level
	if (AlienArray.Num() == 0)
	{
		NextLevel = true;
	}

	// Ødelegg alle skip og reset variablene
	if (NextLevel)
	{
		DestroyAllShips();
		SpawnAllShips();
		NextLevel = false;
		SecondsPerStep = 1.0f;
		StepCounter = 0.f;
		Direction = true;
	}

	// Finn et tall mellom 0 og 100000
	float randSpawn = FMath::RandRange(0, 100000);

	// Veldig rare å få et Mystery skip
	if (randSpawn > 99990)
	{
		SpawnMysteryShip();
	}

	// Sjekk om det er noen AlienSkip som må slettes
	for (AAlienShipPreset* Alien : AlienArray)
	{
		if (Alien->needDelete)
		{
			// Lydeffekt
			UGameplayStatics::PlaySound2D(GetWorld(), AlienDieSound, 1.f, 1.f, 0.f, nullptr);
			// Partikkeleffekt
			CreateExplosionParticleEffect(Alien->GetActorTransform());
			// Fjern fra arrayet
			RemoveActor(Alien);
			// Faktisk fjern den fra spillet
			Alien->Destroy();
			break;	// kill loop
		}
	}

	// Sjekk om Mystery Skip må slettes
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

	if (ourHUD->GameOver)
		return;

	// Hvis det er på tide å hoppe
	if (StepCounter >= SecondsPerStep)
	{
		// Move logic...
		AAlienShipPreset* LeftAlien = GetMostLeftAlien();
		AAlienShipPreset* RightAlien = GetMostRightAlien();

		// Pointer guards
		if (!LeftAlien)
			return;
		if (!RightAlien)
			return;

		// reset counteren
		StepCounter = 0.f;

		// Spillet blir raskere jo mindre aliens det er
		SecondsPerStep = 1.f - (1.f / (0.1*AlienArray.Num() + 1.f));

		// Hvilken retning går vi
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

	// Sjekk om vi får "bonus-liv"
	if ((ourHUD->GetScore() - LastLevelScore) > RequiredPoints)
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
	// Spawn explosjons effekt
	t.SetScale3D(FVector(0.4f, 0.4f, 0.4f));
	auto MyDeferredActor = Cast<AExplosion>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AExplosion::StaticClass(), t));

	if (MyDeferredActor)
	{
		UGameplayStatics::FinishSpawningActor(MyDeferredActor, t);
	}
}

void AGameModeClass::SpawnMysteryShip()
{
	// Spawn et mystery skip med en tilfeldig poengsum (50, 100, 150, 300)
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

	// Når vi er ferdig med å flytte alle actors så er det ok for alle Aliens å raycastsjekke om de kan skyte
	// Gjør sånn at de ikke skyter på hverandre ( så ofte )
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
	// Spawn alle skip
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