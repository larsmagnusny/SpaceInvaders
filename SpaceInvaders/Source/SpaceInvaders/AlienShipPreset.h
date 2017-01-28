// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AlienShipPreset.generated.h"

UCLASS()
class SPACEINVADERS_API AAlienShipPreset : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlienShipPreset();
	~AAlienShipPreset();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Brukt for å sette posisjon fra en annen classe
	void SetPosition(const FVector Location);

	// Sjekk om det finnes et skip rett foran deg
	const FHitResult GetFirstShipInReach();

	// Skyt et skudd
	void Fire();

	// Hent posisjonen fra en annen klasse
	FVector GetPosition();

	// Brukt for å initializere klassen i Deferred spawning
	void SetMeshNum(int32 n);

	// Alle Aliens blir slettet i GameModeClass.cpp
	// Derfor trenger vi en måte å sjekke om de må slettes
	bool needDelete = false;

	// Denne settes til false hvis Det er et mystery skip
	// og den har timet ut og må slettes
	bool DeathByPlayer = true;

	// Mesh for forskjellige typer skip
	UStaticMesh* Mesh1;
	UStaticMesh* Mesh2;
	UStaticMesh* Mesh3;
	UStaticMesh* Mesh4;

	// Materialer for forskjellige typer skip
	UMaterial* MeshMaterial1;
	UMaterial* MeshMaterial1_2;
	UMaterial* MeshMaterial2;
	UMaterial* MeshMaterial2_2;
	UMaterial* MeshMaterial3;
	UMaterial* MeshMaterial3_2;
	UMaterial* MeshMaterial4;

	// Lydeffekter
	USoundWave* DieSound;
	USoundWave* MysterySound;

	UAudioComponent* MysteryPlayer;

	// Getters and setters
	int32 GetValue()
	{
		return PointValue;
	}

	void SetValue(int32 n)
	{
		PointValue = n;
	}

	void SetCanCheckRayCast(bool checkRayCast)
	{
		canCheckRayCast = checkRayCast;
	}

	bool GetCanCheckRayCast()
	{
		return canCheckRayCast;
	}

private:
	// Parameter som brukes i init av klassen
	int32 MeshNum = 0;
	int32 PointValue = 0;
	bool canCheckRayCast = false;
	bool isMysteryShip = false;

	// Brukes når skipet er et Mystery skip
	float Velocity = 1000.f;
	float AliveTime = 7.f;
	float timeAlive = 0.f;
};
