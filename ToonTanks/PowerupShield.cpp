// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupShield.h"
#include "Tank.h"
#include "HealthComponent.h"

void APowerupShield::BeginPlay()
{
	Super::BeginPlay();

}

void APowerupShield::BeginPowerup()
{
	Super::BeginPowerup();
	
	if (PlayerTank && PlayerTank->IsAlive())
	{
		PlayerTank->HealthComp->SetCanTakeDamage(false);
	}
}

void APowerupShield::EndPowerup()
{
	Super::EndPowerup();
	
	if (PlayerTank && PlayerTank->IsAlive())
	{
		PlayerTank->HealthComp->SetCanTakeDamage(true);
	}
}