// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleDefinitions.h"	// Vi må vite hva et UParticleSystemComponent er
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class SPACEINVADERS_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Keep track of how long you have been alive
	float aliveTime = 2.0f;
	float timeAlive = 0.0f;

	// Pointer til partikkel systemet
	UParticleSystemComponent* ExplosionParticleEmitter;
};
