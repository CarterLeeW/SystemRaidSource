// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupSpeed.h"
#include "Tank.h"
#include "GameFramework/FloatingPawnMovement.h"

void APowerupSpeed::BeginPlay()
{
	Super::BeginPlay();
	if (SpeedCoefficient < 1.0f)
	{
		SpeedCoefficient = 1.0f;
		UE_LOG(LogTemp, Warning, TEXT("PowerupSpeed-SpeedCoefficient must be greater than or equal to 1.0"));
	}
}

void APowerupSpeed::BeginPowerup()
{
	Super::BeginPowerup();

	if (PlayerTank && PlayerTank->IsAlive())
	{
		PlayerTank->GetFloatingPawnMovementComp()->MaxSpeed *= SpeedCoefficient;
	}
	UE_LOG(LogTemp, Display, TEXT("max speed: %f"), PlayerTank->GetFloatingPawnMovementComp()->MaxSpeed);
}

void APowerupSpeed::EndPowerup()
{
	Super::EndPowerup();

	if (PlayerTank && PlayerTank->IsAlive())
	{
		PlayerTank->GetFloatingPawnMovementComp()->MaxSpeed /= SpeedCoefficient;
	}
}