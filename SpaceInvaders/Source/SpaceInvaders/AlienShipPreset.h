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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetPosition(const FVector Location);

	FVector GetPosition();

	void SetMeshNum(int32 n);

	bool needDelete = false;

	UStaticMesh* Mesh1;
	UStaticMesh* Mesh2;
	UStaticMesh* Mesh3;

	UMaterial* MeshMaterial1;
	UMaterial* MeshMaterial2;
	UMaterial* MeshMaterial3;

	int32 GetValue()
	{
		return PointValue;
	}

	void SetValue(int32 n)
	{
		PointValue = n;
	}

private:
	int32 MeshNum = 0;
	int32 PointValue = 0;
};
