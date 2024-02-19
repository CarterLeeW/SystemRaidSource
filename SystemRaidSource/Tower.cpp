// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = true;
	// Components
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	TurretCenter = CreateDefaultSubobject<USceneComponent>(TEXT("Turret Center Point"));
	TurretCenter->SetupAttachment(TurretMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	
	if (PlayerTank)
	{
		if (IsInFireRange())
		{
			// Does this drain performance?
			// create timer for tower upon first occurrence of being in range -- gives each tower a unique timer cycle
			if (!GetWorldTimerManager().TimerExists(CannonTimerHandle) && ActiveAmmoType == EAmmoType::CANNON)
			{
				GetWorldTimerManager().SetTimer(CannonTimerHandle, this, &ATower::CheckFireCondition, CannonFireRate, true);
			}
			else if (!GetWorldTimerManager().TimerExists(MachineGunTimerHandle) && ActiveAmmoType == EAmmoType::MACHINE_GUN)
			{
				GetWorldTimerManager().SetTimer(MachineGunTimerHandle, this, &ATower::CheckFireCondition, MachineGunFireRate, true);
			}

			RotateTurret(PlayerTank->GetTankCenterLocaltion());
		}
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerTank pointer is null"));
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATower::RotateTurret(const FVector& LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretCenter->GetComponentLocation();
	//FRotator LookAtRotation = FRotator(0.0, ToTarget.Rotation().Yaw, 0.0);    // Yaw only
	FRotator LookAtRotation = ToTarget.Rotation();
	if (LookAtRotation.Pitch > MaxTurretPitch)
	{
		LookAtRotation.Pitch = MaxTurretPitch;
	}
	else if (LookAtRotation.Pitch < -MaxTurretPitch)
	{
		LookAtRotation.Pitch = -MaxTurretPitch;
	}
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	TurretMesh->SetWorldRotation(FMath::RInterpTo(
			TurretMesh->GetComponentRotation(), 
			LookAtRotation,
			DeltaTime, 
			TurretRotateLerp));
	//ProjectileSpawnPoint->SetWorldRotation(ToTarget.Rotation());  // don't need this
}

void ATower::Fire()
{
	switch (ActiveAmmoType)
	{
	case EAmmoType::CANNON:
		if (bCanFireCannon)
		{
			GetWorldTimerManager().SetTimer(CannonTimerHandle, this, &ATower::RefreshCannon, CannonFireRate, false);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();               // is this necessary?
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
				CannonProjectileClass,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation(),
				SpawnParams);
			bCanFireCannon = false;
		}
		break;
	case EAmmoType::MACHINE_GUN:
		if (bCanFireMachineGun)
		{
			GetWorldTimerManager().SetTimer(MachineGunTimerHandle, this, &ATower::RefreshMachineGun, MachineGunFireRate, false);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();               // is this necessary?
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
				MachineGunProjectileClass,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation() + FRotator(
					FMath::RandRange(-MachineGunSpread, MachineGunSpread),
					FMath::RandRange(-MachineGunSpread, MachineGunSpread),
					FMath::RandRange(-MachineGunSpread, MachineGunSpread)),
				SpawnParams);
			bCanFireMachineGun = false;
		}
		break;
	default:
		break;
	}
}

void ATower::CheckFireCondition()
{
	if (PlayerTank)
	{	// collapse this into a function eventually
		FHitResult HitResult;
		bool HasHit = SweepForTarget(HitResult);
		if (HasHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if ((HitActor == PlayerTank) && PlayerTank->IsAlive())
			{
				Fire();
			}
		}
	}
	else UE_LOG(LogTemp, Error, TEXT("PlayerTank pointer is null"));
}

bool ATower::IsInFireRange()
{
	float Distance = FVector::Dist(GetActorLocation(), PlayerTank->GetActorLocation());
	return Distance <= FireRange;
}

bool ATower::SweepForTarget(FHitResult& OutHitResult) const
{
	FVector Start = ProjectileSpawnPoint->GetComponentLocation();
	FVector End = Start + (ProjectileSpawnPoint->GetForwardVector() * FireRange);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(10.0f);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Camera,
		Sphere
	);
	return HasHit;
}