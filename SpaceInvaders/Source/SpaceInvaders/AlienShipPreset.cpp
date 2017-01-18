// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "AlienShipPreset.h"
#include "Bullet.h"


// Sets default values
AAlienShipPreset::AAlienShipPreset()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh1(TEXT("StaticMesh'/Game/SpaceShip1'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material1(TEXT("MaterialInstanceConstant'/Game/lambert1'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh2(TEXT("StaticMesh'/Game/SpaceShip2'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material2(TEXT("MaterialInstanceConstant'/Game/lambert1'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh3(TEXT("StaticMesh'/Game/SpaceShip3'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material3(TEXT("MaterialInstanceConstant'/Game/lambert1'"));

	Mesh1 = StaticMesh1.Object;
	Mesh2 = StaticMesh2.Object;
	Mesh3 = StaticMesh3.Object;

	MeshMaterial1 = Material1.Object;
	MeshMaterial2 = Material2.Object;
	MeshMaterial3 = Material3.Object;

	PrimaryActorTick.bCanEverTick = true;
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

	if (FMath::RandRange(0.f, 10000.f) >= 9990)
	{
		FHitResult Hit = GetFirstShipInReach();

		if(!Hit.GetActor())
			Fire();
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
	}
	if (n == 1)
	{
		StaticMeshComp->SetStaticMesh(Mesh2);
		StaticMeshComp->SetMaterial(0, MeshMaterial2);
	}

	if (n == 2)
	{
		StaticMeshComp->SetStaticMesh(Mesh3);
		StaticMeshComp->SetMaterial(0, MeshMaterial3);
	}

	// Set the collision parameters...
	StaticMeshComp->SetCollisionProfileName(FName("Destructible"));		// So that other ships can raytrace to check if they should fire or not...
	StaticMeshComp->bGenerateOverlapEvents = true;
	StaticMeshComp->SetNotifyRigidBodyCollision(true);
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