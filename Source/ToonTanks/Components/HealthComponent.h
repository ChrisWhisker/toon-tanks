// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "HealthComponent.generated.h"

// This is a forward declaration. Do this instead of including the header file when the class members aren't needed in
// this header file. This prevents circular dependency.
class ATankGameModeBase;

// HealthComponent is an ActorComponent, meaning it has no transform in the world
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float DefaultHealth = 200.f;
	float Health = 0.0f;

	ATankGameModeBase* GameModeRef;

public:
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
		AActor* DamageCauser);
};
