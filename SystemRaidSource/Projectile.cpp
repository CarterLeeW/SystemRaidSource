// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Tank.h"
#include "Tower.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComp->InitialSpeed = 1000.0f;
	ProjectileMovementComp->MaxSpeed = 1000.0f;

	SmokeTrailComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	SmokeTrailComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	if (LaunchSound)
	{
		if (GetOwner() == Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0)))
		{
			UGameplayStatics::PlaySound2D(this, LaunchSound);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
		}
	}
	if (LaunchParticles)
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchParticles, GetActorLocation(), GetActorRotation());
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetActorEnableCollision(false);
	BaseMesh->SetHiddenInGame(true);
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &AProjectile::DestroyProjectile, 3.0f, false);

	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{   // Wake rigid bodies here??
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		if (HitCameraShakeClass)
		{
			ATank* PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
			ATower* Tower = Cast<ATower>(OtherActor);
			if ((PlayerTank == OtherActor) || Tower)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
			}
		}
		if (SmokeTrailComp)
		{
			SmokeTrailComp->Deactivate();
		}
	}
}