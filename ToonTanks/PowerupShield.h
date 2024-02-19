// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupBase.h"
#include "PowerupShield.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API APowerupShield : public APowerupBase
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

private:

};
