// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Throwable.generated.h"

class UHealthComponent;

UCLASS()
class TOONTANKS_API AThrowable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UParticleSystem* DeathParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* DeathSound;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ExplosionRadius = 50.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ExplosionSplashDamage = 50.0f;

	void HandleDestruction();

private:
};
