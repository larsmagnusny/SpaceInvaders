// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "Explosion.h"


// Sets default values
AExplosion::AExplosion()
{
	// Last inn partikkel systemet
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("ParticleSystem'/Game/Particle/Explosion.Explosion'"));

	// Lag partikkel systemet som en komponent på vår Actor
	ExplosionParticleEmitter = CreateDefaultSubobject<UParticleSystemComponent>(FName("Explosion_Effect"));

	// Hvis partikkel systemet har blitt lagt så legger vi det til som partikkel system
	if (ParticleSystem.Succeeded())
	{
		ExplosionParticleEmitter->Template = ParticleSystem.Object;
	}

	// Ikke start den med en gang, og gjør den synlig
	ExplosionParticleEmitter->bAutoActivate = false;
	ExplosionParticleEmitter->SetHiddenInGame(false);

	// For å fjerne warning
	RootComponent = ExplosionParticleEmitter;

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	// Spill av partikkel systemet
	ExplosionParticleEmitter->ActivateSystem();
}

// Called every frame
void AExplosion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Ødelegg deg selv når tiden har gått ut
	if (timeAlive > aliveTime)
	{
		Destroy();
	}

	timeAlive += DeltaTime;
}

