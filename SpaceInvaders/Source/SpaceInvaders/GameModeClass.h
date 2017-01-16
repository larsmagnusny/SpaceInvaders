// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	virtual void BeginPlay() override;

	TArray<AActor*> AlienArray;
};
