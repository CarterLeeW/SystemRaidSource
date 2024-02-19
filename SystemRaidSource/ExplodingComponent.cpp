// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UExplodingComponent::UExplodingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UExplodingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UExplodingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UExplodingComponent::Explode()
{
	GetOwner()->SetActorTickEnabled(false);
	GetOwner()->SetActorEnableCollision(false);
	GetOwner()->SetActorHiddenInGame(true);
	ExplosionLocation = GetOwner()->GetActorLocation();

	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, ExplosionLocation, GetOwner()->GetActorRotation());
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, ExplosionLocation);
	}
	// Delay application of damage to stagger chain reactions
	FTimerHandle TimerHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, this, &UExplodingComponent::ApplyDamage, 0.15f, false);
}

void UExplodingComponent::ApplyDamage()
{
	// Apply damage to nearby actors
	FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);
	TArray<FHitResult> HitResults;
	FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_Pawn) | ECC_TO_BITFIELD(ECC_WorldDynamic) | ECC_TO_BITFIELD(ECC_PhysicsBody));
	GetWorld()->SweepMultiByObjectType(
		HitResults,
		ExplosionLocation,
		ExplosionLocation,
		FQuat::Identity,
		QueryParams,
		Sphere
	);

	AController* MyOwnerInstigator = GetOwner()->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();
	for (auto& result : HitResults)
	{
		if (result.GetActor() != GetOwner())  // Damage other nearby actors
		{
			UGameplayStatics::ApplyDamage(result.GetActor(), ExplosionSplashDamage, MyOwnerInstigator, GetOwner(), DamageTypeClass);
		}
	}
	GetOwner()->Destroy();
}