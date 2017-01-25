// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "AlienShipPreset.h"
#include "Bullet.h"


// Sets default values
AAlienShipPreset::AAlienShipPreset()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh1(TEXT("StaticMesh'/Game/SpaceShip1.SpaceShip1'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material1(TEXT("Material'/Game/SpaceShip1_Body.SpaceShip1_Body'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material1_2(TEXT("Material'/Game/SpaceShip1_Window.SpaceShip1_Window'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh2(TEXT("StaticMesh'/Game/SpaceShip2.SpaceShip2'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material2(TEXT("Material'/Game/SpaceShip2_Body.SpaceShip2_Body'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material2_2(TEXT("Material'/Game/SpaceShip2_Window.SpaceShip2_Window'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh3(TEXT("StaticMesh'/Game/SpaceShip3.SpaceShip3'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material3(TEXT("Material'/Game/SpaceShip3_Body.SpaceShip3_Body'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material3_2(TEXT("Material'/Game/SpaceShip3_Ring.SpaceShip3_Ring'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh4(TEXT("StaticMesh'/Game/MysteryShip.MysteryShip'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material4(TEXT("Material'/Game/Mystery_Body.Mystery_Body'"));

	static ConstructorHelpers::FObjectFinder<USoundWave> Myst(TEXT("SoundWave'/Game/Sounds/ufo_lowpitch.ufo_lowpitch'"));

	

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

	if (!isMysteryShip)
	{
		if (FMath::RandRange(0.f, 10000.f) >= 8500 && canCheckRayCast)
		{
			FHitResult Hit = GetFirstShipInReach();

			if (!Hit.GetActor())
				Fire();

			canCheckRayCast = false;
		}
		else {
			canCheckRayCast = false;
		}
	}
	else {
		// We should tick for a while then destroy ourselves...
		if (timeAlive > AliveTime)
		{
			needDelete = true;
			DeathByPlayer = false;
		}

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
	

	UStaticMeshComponent* StaticMeshComp = GetStaticMeshComponent();
	StaticMeshComp->Mobility = EComponentMobility::Movable;
	if (n == 0)
	{
		StaticMeshComp->SetStaticMesh(Mesh1);
		StaticMeshComp->SetMaterial(0, MeshMaterial1);
		StaticMeshComp->SetMaterial(1, MeshMaterial1_2);
	}
	if (n == 1)
	{
		StaticMeshComp->SetStaticMesh(Mesh2);
		StaticMeshComp->SetMaterial(0, MeshMaterial2);
		StaticMeshComp->SetMaterial(1, MeshMaterial2_2);
	}

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

	if (n == 3)
	{
		StaticMeshComp->SetStaticMesh(Mesh4);
		StaticMeshComp->SetMaterial(0, MeshMaterial4);

		isMysteryShip = true;

		Velocity = 3000.0f;

		MysteryPlayer = ConstructObject<UAudioComponent>(UAudioComponent::StaticClass(), this);

		MysteryPlayer->OnComponentCreated();
		MysteryPlayer->RegisterComponent();

		if (MysteryPlayer->bWantsInitializeComponent)
		{
			MysteryPlayer->InitializeComponent();
		}

		MysteryPlayer->Sound = MysterySound;

		MysteryPlayer->bAutoActivate = true;
	}

	
}

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