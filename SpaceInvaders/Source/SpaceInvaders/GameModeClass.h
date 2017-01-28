// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlienShipPreset.h"
#include "SpaceInvadersHUD.h"			// Vi må vite hva en ASpaceInvadersHUD er for noe...
#include "GameFramework/GameModeBase.h"
#include "GameModeClass.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API AGameModeClass : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeClass();
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	virtual void BeginPlay() override;

	// Slett en actor ifra AlienArray
	void RemoveActor(AAlienShipPreset* ActorToDelete);	

	// Spawn en actor på en transform med en poengsum og meshtype
	void SpawnActor(int32 Type, int32 Points, FTransform t);	

	// Flytt på alle elementetene i AlienArray
	void MoveAllActors(FVector ToPos);

	// Slett alle skipene i AlienArray
	void DestroyAllShips();

	// Spawn alle skipene og legg dem i AlienArray
	void SpawnAllShips();

	// Called when all aliens have been destroyed
	void RestartGame();

	// Spill av en tilfeldig lyd
	void PlayRandomMoveSound();

	// Spill av en "eksplosjonseffekt" når et alienskip har blitt ødelagt
	void CreateExplosionParticleEffect(FTransform t);

	// Random 1000 eller 1500 poeng trengs for å få ett ekstra liv
	void SetRequirement();

	// Spawner et mystery skip sånn av å til
	void SpawnMysteryShip();

	// Hent de AlienSkipene som er lengst til høyre og venstre
	AAlienShipPreset* GetMostLeftAlien();
	AAlienShipPreset* GetMostRightAlien();

	// Her holder vi styr på alle AlienSkipene som har blitt spawnet
	TArray<AAlienShipPreset*> AlienArray;

	// Vi må også holde oversyn på Mystery skipene
	TArray<AAlienShipPreset*> MysteryArray;

	// Pointer til HUD-klassen
	ASpaceInvadersHUD* ourHUD = nullptr;
private:
	float StepCounter = 0.0f;		// Tell antall sekund mellom "hopp" av fiender
	float SecondsPerStep = 1.0f;    // Vi starter på 1.0s
	bool Direction = true;			// Hvilken retning vi hopper

	int32 RequiredPoints = 1000;	// Hvor mange poeng trenger vi for å få et ekstra liv
	int32 LastLevelScore = 0;		// Lagre poeng når vi får ett nytt liv

	// Lyder som brukes
	USoundWave* AlienDieSound;
	USoundWave* AlienMoveSound1;
	USoundWave* AlienMoveSound2;
	USoundWave* AlienMoveSound3;
	USoundWave* AlienMoveSound4;

	// Restart level
	bool NextLevel = false;
};
