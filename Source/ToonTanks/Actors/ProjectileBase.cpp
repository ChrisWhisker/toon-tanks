// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "ToonTanks/Pawns/PawnBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// CreateDefaultSubobject<>() creates the actual component on this actor. Should be done in the constructor.
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	// This line is declaring a dynamic delegate. It is saying that when the ProjectileMesh's OnComponentHit event is
	// called, the function in parentheses should be called as well.
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	// Sets the ProjectileMesh as the top of the component hierarchy on this actor.
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed * 3;
	InitialLifeSpan = 3.f;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	// SetupAttachment() is making ParticleTrail a child component of RootComponent.
	ParticleTrail->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	// Play a 3d soound in world space that remains at this location as opposed to SpawnSoundAttached.
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (!MyOwner)
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		// This line applies damage to OtherActor
		// float Damage is the amount of damage.
		// instigated by the owner's pawn
		// cause by this actor (projectile)
		// The damage is of type DamageType.
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());

		APawnBase* HitPawn = Cast<APawnBase>(OtherActor);

		if (HitPawn)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HitShake);
		}

		// Completely delete this actor
		Destroy();
	}
}