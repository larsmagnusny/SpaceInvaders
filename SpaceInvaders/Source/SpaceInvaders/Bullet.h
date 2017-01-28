// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpaceShipController.h"
#include "Engine/StaticMeshActor.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API ABullet : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ABullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void SetPosition(const FVector Location);

	FVector GetPosition();

	// En Event som kalles hver gang en kule-actor overlapper en annen actor
	// Som for-så-vidt betyr at meshen har en kollider på seg
	UFUNCTION()
	void OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor);

	// Vi må vite hvem som fyrte av skudded er for å ikke ødelegge den
	void SetParent(USpaceShipController* Parent)
	{
		SpaceShip = Parent;
	}

	void SetVelocity(float S)
	{
		Speed = S;
	}

	
private:
	float Speed = 12000.0f;	// 1000 cm/s
	float AliveTime = 0.f;	// Should delete itself when its done / misses...
	float MaxAliveTime = 2.0f;

	// Pointer til han som skjøt
	USpaceShipController* SpaceShip = nullptr;
	ASpaceInvadersHUD* ourHUD;
};
