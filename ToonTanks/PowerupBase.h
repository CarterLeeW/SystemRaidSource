// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupBase.generated.h"

UCLASS()
class TOONTANKS_API APowerupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerupBase();

	virtual void Tick(float DeltaTime) override;
	// Functions
	virtual void BeginPowerup();
	virtual void EndPowerup();
	float GetDuration() { return EffectDuration; }
	bool IsAvailable() { return bIsAvailable; }

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTriggerComponent* TriggerComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* NiagaraEffects;

	// Editor Attributes
	UPROPERTY(EditDefaultsOnly, Category = "Status Effects")
	float EffectDuration = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Status Effects")
	float RespawnAfterSec = 3.0f;
	UPROPERTY(EditAnywhere, Category = "Status Effects")
	bool bShouldRespawn = true;
	// Variables
	class ATank* PlayerTank;
	FTimerHandle RespawnTimerHandle;
	bool bIsAvailable = true;

	void RespawnPowerup();
private:	
};
