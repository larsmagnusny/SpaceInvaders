// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvaders.h"
#include "SpaceShipController.h"
#include "Bullet.h"


// Sets default values for this component's properties
USpaceShipController::USpaceShipController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Load sounds the object will use...
	static ConstructorHelpers::FObjectFinder<USoundWave> FireSndWave(TEXT("SoundWave'/Game/Sounds/shoot.shoot'"));
	FireSound = FireSndWave.Object;
}


// Called when the game starts
void USpaceShipController::BeginPlay()
{
	Super::BeginPlay();

	ourGameMode = Cast<AGameModeClass>(GetWorld()->GetAuthGameMode());

	// Set up Collisions
	GetOwner()->OnActorBeginOverlap.AddDynamic(this, &USpaceShipController::OnOverlapBegin);

	// Set up KeyBindings
	UInputComponent* InputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find the input controller!?"));
	}
	else {
		InputComponent->BindAction(FName("Left"), IE_Pressed, this, &USpaceShipController::MoveLeft);
		InputComponent->BindAction(FName("Left"), IE_Released, this, &USpaceShipController::StopMoveLeft);

		InputComponent->BindAction(FName("Right"), IE_Pressed, this, &USpaceShipController::MoveRight);
		InputComponent->BindAction(FName("Right"), IE_Released, this, &USpaceShipController::StopMoveRight);

		InputComponent->BindAction(FName("Fire"), IE_Pressed, this, &USpaceShipController::Fire);
		InputComponent->BindAction(FName("NewGame"), IE_Pressed, this, &USpaceShipController::RestartGame);
	}
}

void USpaceShipController::RestartGame()
{
	if (!ourHUD)
		return;
	if (!ourGameMode)
		return;

	if (ourHUD->GetGameOver())
		ourGameMode->RestartGame();
}

void USpaceShipController::InitHUD()
{
	ourHUD = Cast<ASpaceInvadersHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void USpaceShipController::MoveLeft()
{
	MovingLeft = true;
}

void USpaceShipController::StopMoveLeft()
{
	MovingLeft = false;
}

void USpaceShipController::MoveRight()
{
	MovingRight = true;
}

void USpaceShipController::StopMoveRight()
{
	MovingRight = false;
}

void USpaceShipController::Fire()
{
	if (FireCooldown)
		return;

	//UGameplayStatics::PlaySound2D(GetWorld(), USoundBase())

	UGameplayStatics::PlaySound2D(GetWorld(), FireSound, 1.f, 1.f, 0.f, nullptr);

	FVector ourLocation = GetOwner()->GetActorLocation();

	FTransform t = FTransform();
	t.SetLocation(ourLocation + FVector(0.f, 500.f, 0));
	t.SetScale3D(FVector(16.f, 16.f, 16.f));
	t.SetRotation(FQuat::MakeFromEuler(FVector(0.0f, 0.0f, -90.f)));

	auto MyDeferredActor = Cast<ABullet>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ABullet::StaticClass(), t));

	if (MyDeferredActor)
	{
		MyDeferredActor->SetParent(this);

		UGameplayStatics::FinishSpawningActor(MyDeferredActor, t);
	}

	FireCooldown = true;
}

// Called every frame
void USpaceShipController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	Rotation = GetOwner()->GetActorRotation();

	if (FireCooldown)
	{
		ShotTimer += DeltaTime;

		if (ShotTimer >= TimePerShot)
		{
			FireCooldown = false;
			ShotTimer = 0.f;
		}
	}

	if (MovingLeft && MovingRight)
	{
		// Do nothing here, because the movements cancel eachother out...
		return;
	}

	if (!MovingLeft && !MovingRight)
	{
		if (Speed > 0.f)
			Speed -= Acceleration*DeltaTime*0.5;
		if (Speed < 0.f)
			Speed += Acceleration*DeltaTime*0.5;

		if (Rotation.Pitch > 0.f)
			Rotation.Add(-RollSpeed*DeltaTime*0.6, 0.f, 0.f);
		if (Rotation.Pitch < 0.f)
			Rotation.Add(RollSpeed*DeltaTime*0.6, 0.f, 0.f);
	}
	else {
		if (Speed < 0.f && MovingLeft)
			Speed += Acceleration*DeltaTime;
		if (Speed > 0.f && MovingRight)
			Speed -= Acceleration*DeltaTime;

		if (Rotation.Pitch > 0.f && MovingLeft)
			Rotation.Add(-RollSpeed*DeltaTime, 0.f, 0.f);
		if (Rotation.Pitch < 0.f && MovingRight)
			Rotation.Add(RollSpeed*DeltaTime, 0.f, 0.f);
	}

	if (MovingLeft) {
		Speed += Acceleration*DeltaTime;
		if (Rotation.Pitch > -MaxRoll) {
			Rotation.Add(-RollSpeed*DeltaTime, 0.f, 0.f);
		}
		else {
			Rotation = FRotator(-MaxRoll, 0.f, 0.f);
		}
	}

	if (MovingRight)
	{
		Speed -= Acceleration*DeltaTime;
		if (Rotation.Pitch < MaxRoll) {
			Rotation.Add(RollSpeed*DeltaTime, 0.f, 0.f);
		}
		else {
			Rotation = FRotator(MaxRoll , 0.f, 0.f);
		}
	}

	FTransform ShipTransform = GetOwner()->GetTransform();
	FVector ShipPosition = ShipTransform.GetLocation();

	if (ShipPosition.X > -10000.0f && ShipPosition.X < 10000.0f)
	{
		// Add to the X axis...
		if (Speed > TopSpeed)
			Speed = TopSpeed;
		if (Speed < -TopSpeed)
			Speed = -TopSpeed;

		ShipPosition.X += Speed*DeltaTime;
		ShipTransform.SetLocation(ShipPosition);
		GetOwner()->SetActorTransform(ShipTransform, false, nullptr, ETeleportType::None);
		GetOwner()->SetActorRotation(Rotation);
	}
	else {
		Speed = 0.f;

		if (ShipPosition.X < -10000.f)
			ShipPosition.X = -9999.f;
		if (ShipPosition.X > 10000.f)
			ShipPosition.X = 9999.f;

		ShipTransform.SetLocation(ShipPosition);
		GetOwner()->SetActorTransform(ShipTransform, false, nullptr, ETeleportType::None);
		GetOwner()->SetActorRotation(Rotation);
	}
}

void USpaceShipController::OnOverlapBegin(AActor* MyOverlappedActor, AActor* OtherActor)
{
	OtherActor->Destroy();
	if (!ourHUD) {
		InitHUD();
	}
	int32 ourLives = ourHUD->GetLives();

	if (ourLives > 0) {
		--ourLives;
		ourHUD->SetLives(ourLives);
	}
	else
	{
		// We should be dead now!
		ourHUD->SetGameOver(true);
		ourHUD->SetHighScore(ourHUD->GetScore());
	}
}