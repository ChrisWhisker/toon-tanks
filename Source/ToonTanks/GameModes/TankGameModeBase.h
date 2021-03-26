// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

class APawnTurret;
class APawnTank;
class APlayerControllerBase;

// A game mode defines the game being played. It governs the game rules, scoring, etc.
UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	APawnTank* PlayerTank;
	int32 TargetTurrets = 0;
	APlayerControllerBase* PlayerControllerRef;
	int32 GetTargetTurretCount();
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);

public:
	void ActorDied(AActor* DeadActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop")
	int32 StartDelay = 3;

	virtual void BeginPlay() override;

	// These two functions don't need to be implemented in C++ because they are implemented in the child blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bPlayerWon);
};