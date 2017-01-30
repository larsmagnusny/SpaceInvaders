// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "AlienShipPreset.h"	// Trenger å vite hva en AAlienShipPreset er for å calle object.needDelete = true;
#include "Bullet.h"


ABullet::ABullet()
{
	// Last inn meshen til kulen og materialet til kulen
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Models/Bullet.Bullet'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/Bullet_Mat.Bullet_Mat'"));

	// Når vi spawner objektet så har den allerede en staticMeshComponent fordi denne klassen arver ifra den
	UStaticMeshComponent* StaticMeshComp = GetStaticMeshComponent();

	StaticMeshComp->SetStaticMesh(StaticMesh.Object);
	StaticMeshComp->SetMaterial(0, Material.Object);

	StaticMeshComp->bGenerateOverlapEvents = true;
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
	StaticMeshComp->Mobility = EComponentMobility::Movable;

	StaticMeshComp->SetCollisionProfileName(FName("OverlapAll"));
	StaticMeshComp->MoveIgnoreActors.Add(this);
	
	// Si til Unreal at denne funksjonen kalles når kulen overlapper med noe annet
	OnActorBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	// Hent en peker til hudden så vi kan oppdatere poengsum
	ourHUD = Cast<ASpaceInvadersHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Sålenge tiden ikke er ute så beveger vi oss
	if (AliveTime < MaxAliveTime)
	{
		AliveTime += DeltaTime;

		FVector ourLoc = GetActorLocation();
		ourLoc.Y += Speed*DeltaTime;
		SetActorLocation(ourLoc);
	}
	else {
		Destroy();	// Slett deg selv.
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
	ourHUD->collisionEventWasFired = true;
	if (OtherActor && MyOverlappedActor)
	{
		AAlienShipPreset* test = (AAlienShipPreset*)OtherActor;

		// Sjekk om klassen faktisk er en AAlienShipPreset
		if (OtherActor->IsA(AAlienShipPreset::StaticClass()))
		{
			
			// Sjekk om vi ikke har nullpointere
			if (ourHUD && test)
			{
				// Legg til poeng her
				int32 CurPoints = ourHUD->GetScore();

				ourHUD->SetScore(CurPoints + test->GetValue());
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Sorry! Can't find the HUD Class?!"));
			}

			// Si til objektet at den trenger å slettes
			if (test)
				test->needDelete = true;

			// Slett kulen
			MyOverlappedActor->Destroy(false, false);
		}
		// Hvis det er en actor eller en kule vi overlapper
		else if (OtherActor->IsA(AActor::StaticClass()) || OtherActor->IsA(ABullet::StaticClass()))
		{
			// Dum måte å ikke "slette" skipet, siden den teknisk sett er en Actor
			if (!OtherActor->GetName().Equals("SpaceShip_2"))
			{
				MyOverlappedActor->Destroy();
				OtherActor->Destroy();
			}
			else {
				// Slett kulen hvis det er spilleren sitt skip vi sletter
				MyOverlappedActor->Destroy();
			}
		}
	}
}