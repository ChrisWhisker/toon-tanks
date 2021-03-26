// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

class APawnTank;

UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
	GENERATED_BODY()

private:
	void CheckFireCondition();
	float ReturnDistanceToPlayer();

	FTimerHandle FireRateTimerHandle; // Unique identifier for the timer
	APawnTank* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRange = 500.f;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void HandleDestruction() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
