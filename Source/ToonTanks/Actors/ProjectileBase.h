// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
	GENERATED_BODY()

private:
	// ==== Components ====
	// VisibleAnywhere means it is read-only in all property windows
	// BlueprintReadOnly means the variable is available as read-only within the blueprint event graph
	// meta = (AllowPrivateAccess = "true") allows private variables to be accessible to blueprints
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleTrail;

	// ==== Variables ====
	// EditDefaultsOnly means it's editable only on blueprints (not instances of the blueprint).
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage = 50;

	// EditAnywhere means it can be edited in propoerty windows, blueprints, and instances
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 1300.f;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* LaunchSound;
	// TSubclassOf<> is a template that lets you specify the variable is a specific class or subclass of it.
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCameraShake> HitShake;

	// ==== Functions ====
	// UFUNCTION Exposes the function to Unreal
	// TODO is this overriding???
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

public:
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
