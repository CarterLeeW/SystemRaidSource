// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupBase.h"
#include "PowerupHealth.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API APowerupHealth : public APowerupBase
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;

	// Functions
	// Overlap
	virtual void BeginPowerup() override;
	virtual void EndPowerup() override;

	// Editor Properties
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float HealAmount = 50.0f;

private:

};
