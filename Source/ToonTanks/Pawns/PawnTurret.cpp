// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTurret.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange)
	{
		return;
	}

	FHitResult TraceHitResult;
	GetWorld()->LineTraceSingleByChannel(
		TraceHitResult, ProjectileSpawnPoint->GetComponentLocation(), PlayerPawn->GetActorLocation(), ECC_Visibility);

	UE_LOG(LogTemp, Warning, TEXT("%s seen"), *TraceHitResult.GetActor()->GetName());

	APawnTank* PlayerSeen = Cast<APawnTank>(TraceHitResult.GetActor());

	if (PlayerSeen)
	{
		RotateTurret(PlayerPawn->GetActorLocation());
		// UE_LOG(LogTemp, Warning, TEXT("%s seen"), *PlayerSeen->GetName());
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