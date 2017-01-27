// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "Explosion.h"


// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("ParticleSystem'/Game/Particle/Explosion.Explosion'"));

	ExplosionParticleEmitter = CreateDefaultSubobject<UParticleSystemComponent>(FName("Explosion_Effect"));

	if (ParticleSystem.Succeeded())
	{
		ExplosionParticleEmitter->Template = ParticleSystem.Object;
	}

	ExplosionParticleEmitter->bAutoActivate = false;
	ExplosionParticleEmitter->SetHiddenInGame(false);

	RootComponent = ExplosionParticleEmitter;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	ExplosionParticleEmitter->ActivateSystem();
}

// Called every frame
void AExplosion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (timeAlive > aliveTime)
	{
		Destroy();
	}

	timeAlive += DeltaTime;
}

