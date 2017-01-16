// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "AlienShipPreset.h"


// Sets default values
AAlienShipPreset::AAlienShipPreset()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/SpaceShip1'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("MaterialInstanceConstant'/Game/lambert1'"));

	UStaticMeshComponent* StaticMeshComp = GetStaticMeshComponent();

	StaticMeshComp->SetStaticMesh(StaticMesh.Object);
	StaticMeshComp->SetMaterial(0, Material.Object);
	
}

// Called when the game starts or when spawned
void AAlienShipPreset::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlienShipPreset::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AAlienShipPreset::SetPosition(const FVector Location)
{
	SetActorLocation(Location);
}

FVector AAlienShipPreset::GetPosition()
{
	return GetTransform().GetLocation();
}
