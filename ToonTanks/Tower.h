// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATower();
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override;
protected:
	void RotateTurret(const FVector& LookAtTarget);

	// Components
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	USceneComponent* TurretCenter;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> CannonProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> MachineGunProjectileClass;

	// Fire Rate Handling
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle CannonTimerHandle;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle MachineGunTimerHandle;
	UPROPERTY(BlueprintReadOnly)
	bool bCanFireCannon = true;
	void RefreshCannon() { bCanFireCannon = true; }
	bool bCanFireMachineGun = true;
	void RefreshMachineGun() { bCanFireMachineGun = true; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAmmoType ActiveAmmoType = EAmmoType::CANNON;
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float FireRange = 300.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float CannonFireRate = 2.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float MachineGunFireRate = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float MachineGunSpread = 2.0f;

	void Fire();
private:
	class ATank* PlayerTank;


	void CheckFireCondition();
	bool IsInFireRange();
	bool SweepForTarget(FHitResult& OutHitResult) const;
};
