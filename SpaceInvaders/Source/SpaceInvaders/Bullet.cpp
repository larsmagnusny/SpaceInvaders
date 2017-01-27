// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "AlienShipPreset.h"
#include "Bullet.h"
#include "GameModeClass.h"

ABullet::ABullet()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Models/Bullet.Bullet'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/Bullet_Mat.Bullet_Mat'"));

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

	ourHUD = Cast<ASpaceInvadersHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
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

	if (OtherActor && MyOverlappedActor)
	{
		AAlienShipPreset* test = (AAlienShipPreset*)OtherActor;


		if (OtherActor->IsA(AAlienShipPreset::StaticClass()))
		{
			if (ourHUD && test)
			{
				int32 CurPoints = ourHUD->GetScore();

				ourHUD->SetScore(CurPoints + test->GetValue());
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Sorry! Can't find the HUD Class?!"));
			}

			if (test)
				test->needDelete = true;

			MyOverlappedActor->Destroy(false, false);
		}
		else if (OtherActor->IsA(AActor::StaticClass()) || OtherActor->IsA(ABullet::StaticClass()))
		{
			if (!OtherActor->GetName().Equals("SpaceShip_2"))
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), *OtherActor->GetName());
				MyOverlappedActor->Destroy();
				OtherActor->Destroy();
			}
			else {
				MyOverlappedActor->Destroy();
			}
		}
	}
}