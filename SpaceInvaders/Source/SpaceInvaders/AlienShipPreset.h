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

	void SetPosition(const FVector Location);

	const FHitResult GetFirstShipInReach();

	void Fire();

	FVector GetPosition();

	void SetMeshNum(int32 n);

	bool needDelete = false;

	bool DeathByPlayer = true;

	UStaticMesh* Mesh1;
	UStaticMesh* Mesh2;
	UStaticMesh* Mesh3;
	UStaticMesh* Mesh4;

	UMaterial* MeshMaterial1;
	UMaterial* MeshMaterial1_2;
	UMaterial* MeshMaterial2;
	UMaterial* MeshMaterial2_2;
	UMaterial* MeshMaterial3;
	UMaterial* MeshMaterial3_2;
	UMaterial* MeshMaterial4;

	USoundWave* DieSound;
	USoundWave* MysterySound;

	UAudioComponent* MysteryPlayer;

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
	int32 MeshNum = 0;
	int32 PointValue = 0;
	bool canCheckRayCast = false;
	bool isMysteryShip = false;
	float Velocity = 1000.f;
	float AliveTime = 7.f;
	float timeAlive = 0.f;
};
