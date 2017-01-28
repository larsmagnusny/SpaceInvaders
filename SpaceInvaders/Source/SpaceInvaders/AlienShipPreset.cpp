// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "AlienShipPreset.h"
#include "Bullet.h"


// Sets default values
AAlienShipPreset::AAlienShipPreset()
{
	// Her så laster vi inn alle mesher, materialer og lyder vi kanskje kommer til å bruke på alle AlienShips
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh1(TEXT("StaticMesh'/Game/Models/SpaceShip1.SpaceShip1'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material1(TEXT("Material'/Game/Materials/SpaceShip1_Body.SpaceShip1_Body'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material1_2(TEXT("Material'/Game/Materials/SpaceShip1_Window.SpaceShip1_Window'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh2(TEXT("StaticMesh'/Game/Models/SpaceShip2.SpaceShip2'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material2(TEXT("Material'/Game/Materials/SpaceShip2_Body.SpaceShip2_Body'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material2_2(TEXT("Material'/Game/Materials/SpaceShip2_Window.SpaceShip2_Window'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh3(TEXT("StaticMesh'/Game/Models/SpaceShip3.SpaceShip3'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material3(TEXT("Material'/Game/Materials/SpaceShip3_Body.SpaceShip3_Body'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material3_2(TEXT("Material'/Game/Materials/SpaceShip3_Ring.SpaceShip3_Ring'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh4(TEXT("StaticMesh'/Game/Models/MysteryShip.MysteryShip'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material4(TEXT("Material'/Game/Materials/Mystery_Body.Mystery_Body'"));

	static ConstructorHelpers::FObjectFinder<USoundWave> Myst(TEXT("SoundWave'/Game/Sounds/ufo_lowpitch.ufo_lowpitch'"));

	// Sett pekerne til adressen til de objektene vi har lastet inn
	Mesh1 = StaticMesh1.Object;
	Mesh2 = StaticMesh2.Object;
	Mesh3 = StaticMesh3.Object;
	Mesh4 = StaticMesh4.Object;

	MeshMaterial1 = Material1.Object;
	MeshMaterial1_2 = Material1_2.Object;
	MeshMaterial2 = Material2.Object;
	MeshMaterial2_2 = Material2_2.Object;
	MeshMaterial3 = Material3.Object;
	MeshMaterial3_2 = Material3_2.Object;
	MeshMaterial4 = Material4.Object;

	MysterySound = Myst.Object;

	// Vi bruker Tick som kalles hver frame
	PrimaryActorTick.bCanEverTick = true;
}

AAlienShipPreset::~AAlienShipPreset()
{
	
}

// Called when the game starts or when spawned
void AAlienShipPreset::BeginPlay()
{
	Super::BeginPlay();
}

void AAlienShipPreset::Fire()
{
	// Hent vår posisjon, deretter spawn en kule og sett farten til -6000 cm / s
	FVector ourLocation = GetActorLocation();

	FTransform t = FTransform();
	t.SetLocation(ourLocation + FVector(0.f, -1000.f, 0));
	t.SetScale3D(FVector(16.f, 16.f, 16.f));
	t.SetRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.0f, 90.f)));

	auto MyDeferredActor = Cast<ABullet>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ABullet::StaticClass(), t));

	if (MyDeferredActor)
	{
		MyDeferredActor->SetVelocity(-6000.f);

		UGameplayStatics::FinishSpawningActor(MyDeferredActor, t);
	}
}

// Called every frame
void AAlienShipPreset::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Hvis vi ikke er et mystery skip
	if (!isMysteryShip)
	{
		// Finn et tilfeldig tall og sjekk om vi faktisk kan skyte uten å ødelegge våre egne skip
		if (FMath::RandRange(0.f, 10000.f) >= 8500 && canCheckRayCast)
		{
			FHitResult Hit = GetFirstShipInReach(); // RayCast ( Line-Trace )

			if (!Hit.GetActor())	// Skyt hvis vi ikke traff noe
				Fire();

			canCheckRayCast = false; // Brukes for å synke Tick i GameModeClass
		}
		else {
			canCheckRayCast = false; // Brukes for å synke Tick i GameModeClass
		}
	}
	else {
		// We should tick for a while then destroy ourselves...
		if (timeAlive > AliveTime)
		{
			needDelete = true;
			DeathByPlayer = false;
		}

		// Beveg oss hver frame mot høyre fra venstre
		FVector position = GetActorLocation();

		position.X += Velocity*DeltaTime;

		SetActorLocation(position);

		timeAlive += DeltaTime;
	}
}

void AAlienShipPreset::SetPosition(const FVector Location)
{
	SetActorLocation(Location);
}

FVector AAlienShipPreset::GetPosition()
{
	return GetTransform().GetLocation();
}

void AAlienShipPreset::SetMeshNum(int32 n)
{
	// Setter Mesh basert på n
	UStaticMeshComponent* StaticMeshComp = GetStaticMeshComponent();
	StaticMeshComp->Mobility = EComponentMobility::Movable;

	// Bakerste skip type
	if (n == 0)
	{
		StaticMeshComp->SetStaticMesh(Mesh1);
		StaticMeshComp->SetMaterial(0, MeshMaterial1);
		StaticMeshComp->SetMaterial(1, MeshMaterial1_2);
	}

	// Skip type 2
	if (n == 1)
	{
		StaticMeshComp->SetStaticMesh(Mesh2);
		StaticMeshComp->SetMaterial(0, MeshMaterial2);
		StaticMeshComp->SetMaterial(1, MeshMaterial2_2);
	}

	// Skip type 3
	if (n == 2)
	{
		StaticMeshComp->SetStaticMesh(Mesh3);
		StaticMeshComp->SetMaterial(0, MeshMaterial3);
		StaticMeshComp->SetMaterial(1, MeshMaterial3_2);
	}

	// Set the collision parameters...
	StaticMeshComp->SetCollisionProfileName(FName("Destructible"));		// So that other ships can raytrace to check if they should fire or not...
	StaticMeshComp->bGenerateOverlapEvents = true;
	StaticMeshComp->SetNotifyRigidBodyCollision(true);

	// Mystery skip
	if (n == 3)
	{
		StaticMeshComp->SetStaticMesh(Mesh4);
		StaticMeshComp->SetMaterial(0, MeshMaterial4);

		isMysteryShip = true;

		Velocity = 3000.0f;

		// Lag en ny komponent som spiller av lyd
		MysteryPlayer = NewObject<UAudioComponent>(this, FName("MysterySoundPlayer"));

		MysteryPlayer->OnComponentCreated();
		MysteryPlayer->RegisterComponent();

		if (MysteryPlayer->bWantsInitializeComponent)
		{
			MysteryPlayer->InitializeComponent();
		}

		MysteryPlayer->Sound = MysterySound;

		MysteryPlayer->bAutoActivate = true;	// spill av lyd automatisk
	}
}

// Basic Line-Trace
const FHitResult AAlienShipPreset::GetFirstShipInReach()
{
	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("")), false, this);

	// Line-trace out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetActorLocation(),
		GetActorLocation() + FVector(0.f, -10000.f, 0),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Destructible),
		QueryParams
	);

	return Hit;
}