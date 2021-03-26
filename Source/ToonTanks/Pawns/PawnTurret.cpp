// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTurret.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

#define OUT

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	// Starts the repeating timer to call CheckFireCondition() every [FireRate] seconds
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange) // Nothing to fire at or player is out of range
	{
		return;
	}

	// Make sure walls aren't blocking the turret's sight of the player (probably shouldn't be done in tick)
	FHitResult TraceHitResult;
	GetWorld()->LineTraceSingleByChannel(OUT TraceHitResult, ProjectileSpawnPoint->GetComponentLocation(),
		PlayerPawn->GetActorLocation(), ECC_Visibility);

	APawnTank* PlayerSeen = Cast<APawnTank>(TraceHitResult.GetActor());

	if (PlayerSeen) // if the first object hit by the turret's line of sight is the player
	{
		RotateTurret(PlayerPawn->GetActorLocation());
	}
}

void APawnTurret::CheckFireCondition()
{
	if (!PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
	{
		return;
	}

	if (ReturnDistanceToPlayer() <= FireRange)
	{
		Fire();
	}
}

float APawnTurret::ReturnDistanceToPlayer()
{
	if (!PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
	{
		return 0.f;
	}

	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}