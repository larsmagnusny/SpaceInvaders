// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "AlienShip_Third.generated.h"

/**
 * 
 */
UCLASS()
class SPACEINVADERS_API AAlienShip_Third : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AAlienShip_Third();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void SetPosition(const FVector Location);

	FVector GetPosition();
};