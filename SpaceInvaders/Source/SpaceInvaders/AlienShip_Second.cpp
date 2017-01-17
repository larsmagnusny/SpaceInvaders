// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "AlienShip_Second.h"

// Sets default values
AAlienShip_Second::AAlienShip_Second()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/SpaceShip2'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("MaterialInstanceConstant'/Game/lambert1'"));

	UStaticMeshComponent* StaticMeshComp = GetStaticMeshComponent();

	StaticMeshComp->SetStaticMesh(StaticMesh.Object);
	StaticMeshComp->SetMaterial(0, Material.Object);

	StaticMeshComp->bGenerateOverlapEvents = true;
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
	StaticMeshComp->Mobility = EComponentMobility::Movable;

}

// Called when the game starts or when spawned
void AAlienShip_Second::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAlienShip_Second::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAlienShip_Second::SetPosition(const FVector Location)
{
	SetActorLocation(Location);
}

FVector AAlienShip_Second::GetPosition()
{
	return GetTransform().GetLocation();
}
