// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupBase.h"
#include "PowerupSpeed.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API APowerupSpeed : public APowerupBase
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;

	// Functions
	// Overlap
	virtual void BeginPowerup() override;
	virtual void EndPowerup() override;

	// Editor Attributes
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float SpeedCoefficient = 2.0f;
private:

};