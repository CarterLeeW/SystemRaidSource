// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "InputActionValue.h"
#include "Tank.generated.h"


class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class AToonTanksPlayerController;
class UFloatingPawnMovement;
class APowerupBase;
class UBoxComponent;
class UPhysicsHandleComponent;
class UGrabberComponent;

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GetFloatingPawnMovement();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enhanced Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* PivotAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* ZoomAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* SwapAmmoTypeAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* GrabAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* ReleaseAction;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float PivotConstant = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float AimSensitivityDefault = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")     // Overwritten in BeginPlay by editor setting
	float AimSensitivityCurrent = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float AimSensitivityZoom = 50.0f;

	UPROPERTY(BlueprintReadWrite)
	UFloatingPawnMovement* FloatingPawnMovementComp;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BaseHitBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TurretHitBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TankCenter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TurretCenter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsHandleComponent* PhysicsHandleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGrabberComponent* GrabberComp;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> CannonProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> MachineGunProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MaxTankPitch = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MaxTankRoll = 20.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float CannonFireRate = 2.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float MachineGunFireRate = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float MachineGunSpread = 2.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float ThrowStrength = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* SwitchToCannonSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* SwitchToMachineGunSound;
	// Spring Arm
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float SpringArmZoomLength = 200.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	float SpringArmRegularLength = 400.0f; // Overwritten in BeginPlay by editor setting

	void LerpTurretRotation();
	void SnapTurretRotation();
	void RotateTurretToController();

	// Input Functions
	void Move(const FInputActionValue& Value);
	void Pivot(const FInputActionValue& Value);
	void AimTurret(const FInputActionValue& Value);
	void Fire();
	void SwapAmmoType();
	void Grab();
	void Release();
	void Throw();
	// Unused - Implemented in blueprints instead
	void Zoom(const FInputActionValue& Value);
	
	// Grabber
	FTimerHandle CanGrabTimerHandle;
	bool bIsGrabbing = false;
	bool bCanGrab = true;

	// Overlap
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UFloatingPawnMovement* GetFloatingPawnMovementComp() { return FloatingPawnMovementComp; }

	// Powerup Handling
	UPROPERTY(EditAnywhere, Category = "Status Effects")
	APowerupBase* ActivePowerup;
	FTimerHandle PowerupDurationHandle;
	void ConsumePowerup(APowerupBase* NewPowerup);
	void RemovePowerup(APowerupBase* OldPowerup);

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

	UPROPERTY(BlueprintReadOnly)
	EAmmoType ActiveAmmoType = EAmmoType::CANNON;
	UPROPERTY(BlueprintReadOnly)
	EAmmoType LastActiveAmmoType = EAmmoType::CANNON;

	// Crosshair Widget
	UFUNCTION(BlueprintCallable)
	FVector GetCrosshairTraceEnd() const { return CrosshairTraceEnd; }

	virtual void HandleDestruction() override;
	AToonTanksPlayerController* GetTankPlayerController() const;
	bool IsAlive() const { return bIsAlive; }
	FVector GetTankCenterLocaltion() const { return TankCenter ? TankCenter->GetComponentLocation() : GetActorLocation(); }
private:


	AToonTanksPlayerController* TankPlayerController;

	// Crosshair
	FVector CrosshairTraceEnd;


	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	bool bIsAlive;
	void CorrectTankRotation();
};
