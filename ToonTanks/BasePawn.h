// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UENUM()
enum class EAmmoType : uint8 {
	CANNON,
	MACHINE_GUN,
	GRABBER
};

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void HandleDestruction();

	// Components
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	class UHealthComponent* HealthComp;

protected:
	// Editor Attributes
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float TurretRotateLerp = 5.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MaxTurretPitch = 15.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MaxTurretRoll = 15.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* DeathParticles;
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;
};
