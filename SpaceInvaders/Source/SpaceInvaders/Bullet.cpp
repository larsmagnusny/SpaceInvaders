// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "Bullet.h"
#include "GameModeClass.h"

ABullet::ABullet()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Bullet'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("MaterialInstanceConstant'/Game/lambert1'"));

	UStaticMeshComponent* StaticMeshComp = GetStaticMeshComponent();

	StaticMeshComp->SetStaticMesh(StaticMesh.Object);
	StaticMeshComp->SetMaterial(0, Material.Object);

	StaticMeshComp->bGenerateOverlapEvents = true;
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
	StaticMeshComp->Mobility = EComponentMobility::Movable;

	StaticMeshComp->SetCollisionProfileName(FName("OverlapAll"));
	StaticMeshComp->MoveIgnoreActors.Add(this);
	
	OnActorBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AliveTime < MaxAliveTime)
	{
		AliveTime += DeltaTime;

		FVector ourLoc = GetActorLocation();
		ourLoc.Y += Speed*DeltaTime;
		SetActorLocation(ourLoc);
	}
	else {
		K2_DestroyActor();
		UE_LOG(LogTemp, Warning, TEXT("Bullet has been destroyed..."));
	}
}

void ABullet::SetPosition(const FVector Location)
{
	SetActorLocation(Location);
}

FVector ABullet::GetPosition()
{
	return GetTransform().GetLocation();
}


void ABullet::OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor)
{
	FString OverLappingActor = OtherActor->GetName();

	UE_LOG(LogTemp, Warning, TEXT("Im Overlapping: %s"), *OverLappingActor);

	if (OtherActor && MyOverlappedActor)
	{
		OtherActor->Destroy(false, false);
		MyOverlappedActor->Destroy(false, false);
	}
}