// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlienShipPreset.h"
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
	AGameModeClass();			// Maybe i can pass the correct mesh to the correct ship from here!?
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	virtual void BeginPlay() override;

	void RemoveActor(AAlienShipPreset* ActorToDelete);

	void SpawnActor(int32 Type, FTransform t);

	void MoveAllActors(FVector ToPos);

	void DestroyAllShips();
	void SpawnAllShips();

	TArray<AAlienShipPreset*> AlienArray;

private:
	float StepCounter = 0.0f;
	float SecondsPerStep = 1.0f;
	bool Direction = true;
	int32 Steps = 6;
	int32 TotalSteps = 0;
	int32 DownSteps = 10;

	bool GameOver = false;
};
