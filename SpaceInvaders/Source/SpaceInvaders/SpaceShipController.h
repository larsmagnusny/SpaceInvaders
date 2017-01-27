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

	void MoveLeft();
	void StopMoveLeft();
	void MoveRight();
	void StopMoveRight();
	void Fire();
	void Die();
	void RestartGame();
	void InitHUD();

	void CreateExplosionParticleEffect(FTransform t);

	UFUNCTION()
	void OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor);

	FRotator Rotation;

private:
	ASpaceInvadersHUD* ourHUD = nullptr;
	AGameModeClass* ourGameMode = nullptr;

	USoundWave* FireSound = nullptr;
	USoundWave* DieSound = nullptr;

	bool MovingLeft = false;
	bool MovingRight = false;

	bool FireCooldown = false;
	float ShotTimer = 0.f;
	float TimePerShot = 0.5f;

	float Speed = 0.f;
	float TopSpeed = 8000.f;
	float Acceleration = 25000.f;

	float RollSpeed = 120.f;
	float MaxRoll = 45.f;

	bool IsBlinking = false;
	bool visible = true;
	float InvisTime = 0.2f;	// How long will the ship be invisible
	float timeBlink = 0.0f;
	float VisTime = 0.2f;
	int32 numBlinks = 10;
	int32 blinkCounter = 0;
};
