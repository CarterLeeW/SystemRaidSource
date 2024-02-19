// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExplodingComponent.generated.h"

class UHealthComponent;
class USoundCue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UExplodingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExplodingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Explode();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UParticleSystem* DeathParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundCue* DeathSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float ExplosionRadius = 50.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float ExplosionSplashDamage = 50.0f;


private:
	FVector ExplosionLocation;
	void ApplyDamage();
};
