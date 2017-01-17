// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UFUNCTION()
	void OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor);

	
private:
	float Speed = 8000.0f;	// 1000 cm/s
	float AliveTime = 0.f;	// Should delete itself when its done / misses...
	float MaxAliveTime = 2.0f;
};
