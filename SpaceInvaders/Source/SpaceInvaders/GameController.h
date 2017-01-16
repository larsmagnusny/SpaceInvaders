// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "GameController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEINVADERS_API UGameController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameController();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	TArray<AStaticMeshActor*> Aliens;

	UPROPERTY(EditAnywhere)
	float GameSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float StartX = 5420.f;
	
	UPROPERTY(EditAnywhere)
	float StartY = 6770.f;

	UPROPERTY(EditAnywhere)
	float StartZ = 0.f;
};
