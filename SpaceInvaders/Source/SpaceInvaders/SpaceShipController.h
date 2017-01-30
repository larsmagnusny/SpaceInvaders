// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceInvadersHUD.h"
#include "GameModeClass.h"
#include "Components/ActorComponent.h"
#include "SpaceShipController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEINVADERS_API USpaceShipController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpaceShipController();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Bevegelses funksjoner
	void MoveLeft();
	void StopMoveLeft();
	void MoveRight();
	void StopMoveRight();

	void Fire();
	void Die();

	void NewGame();
	
	void InitHUD();
	
	void PauseGame();

	// Lag en eksplosjon hvis du dør
	void CreateExplosionParticleEffect(FTransform t);

	// Sjekk om du kolliderer med noe
	UFUNCTION()
	void OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor);

	// Rotasjons effekt
	FRotator Rotation;
private:
	// Pointere til HUD og GameMode
	ASpaceInvadersHUD* ourHUD = nullptr;
	AGameModeClass* ourGameMode = nullptr;

	// Lydeffekter
	USoundWave* FireSound = nullptr;
	USoundWave* DieSound = nullptr;

	// Brukes i tick
	bool MovingLeft = false;
	bool MovingRight = false;

	// Vi kan ikke skyte uendelig med skudd per sekund
	bool FireCooldown = false;
	float ShotTimer = 0.f;
	float TimePerShot = 0.5f;

	// Fart akselerasjon og topspeed
	float Speed = 0.f;
	float TopSpeed = 8000.f;
	float Acceleration = 25000.f;

	// Roll-variabler når du beveger på deg
	float RollSpeed = 120.f;
	float MaxRoll = 45.f;
	
	// For å blinke og være uskadelig når du har blitt skutt
	bool IsBlinking = false;
	bool visible = true;
	float InvisTime = 0.2f;	// How long will the ship be invisible
	float timeBlink = 0.0f;
	float VisTime = 0.2f;
	int32 numBlinks = 10;
	int32 blinkCounter = 0;
};
