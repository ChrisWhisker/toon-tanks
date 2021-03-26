// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#define OUT

APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	// Necessary in order to accept player input
	PlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotate();
	Move();

	if (PlayerControllerRef)
	{
		FHitResult TraceHitResult;
		// Perform a line trace from the camera in the direction of the cursor on screen
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, OUT TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint; // ImpactPoint is the actual location of the hit.
		RotateTurret(HitLocation);
	}
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Bind input from project settings directly to functions. The Value argument is implicitly passed.
	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value)
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value)
{
	float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation);
}

void APawnTank::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
	bIsPlayerAlive = false;
	SetActorHiddenInGame(true); // Hide the entire actor
	SetActorTickEnabled(false); // Stop ticking to save resources
}

bool APawnTank::GetIsPlayerAlive() // Getter but not setter provided to keep bIsPlayerAlive read-only.
{
	return bIsPlayerAlive;
}