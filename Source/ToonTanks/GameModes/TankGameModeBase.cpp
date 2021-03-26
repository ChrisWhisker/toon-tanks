// Fill out your copyright notice in the Description page of Project Settings.

#include "TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

#define OUT

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void ATankGameModeBase::HandleGameStart()
{
	TargetTurrets = GetTargetTurretCount();
	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GameStart();

	// Casting ensures we have the player controller of class APlayerControllerBase
	PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	if (PlayerControllerRef) // If the cast was successful
	{
		PlayerControllerRef->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableHandle; // Basically a unique identifier for this timer
		// This is how a timer delegate is created
		// Indicates that when this delegate is called, SetPlayerEnabledState(true) will be called on
		// APlayerControllerBase
		FTimerDelegate PlayerEnableDelegate =
			FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState, true);
		// Starts the timer for [StartDelay] seconds without looping
		GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
	}
}

int32 ATankGameModeBase::GetTargetTurretCount()
{
	TArray<AActor*> TurretActors;
	// Very useful to remember
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), OUT TurretActors);
	return TurretActors.Num();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	if (DeadActor == PlayerTank)
	{
		PlayerTank->HandleDestruction();
		HandleGameOver(false);

		if (PlayerControllerRef)
		{
			PlayerControllerRef->SetPlayerEnabledState(false);
		}
	}
	else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
	// If the cast is successful, DeadActor is a turret ^
	{
		DestroyedTurret->HandleDestruction();

		if (--TargetTurrets <= 0) // Decrement first, check value second.
		{
			HandleGameOver(true);
		}
	}
}

void ATankGameModeBase::HandleGameOver(bool bPlayerWon)
{
	GameOver(bPlayerWon);
}
