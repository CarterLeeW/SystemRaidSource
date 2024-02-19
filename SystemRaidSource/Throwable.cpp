// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwable.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AThrowable::AThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

// Called when the game starts or when spawned
void AThrowable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthComp->GetHealth() <= 0.0f)
	{
		HandleDestruction();
	}
}

void AThrowable::HandleDestruction()
{
	// Apply damage to nearby actors
	FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 10.0f, FColor::Red, false, 5.0f);

	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	Destroy();
}