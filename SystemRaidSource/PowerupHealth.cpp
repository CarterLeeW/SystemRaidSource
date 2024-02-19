// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupHealth.h"
#include "Tank.h"
#include "HealthComponent.h"

void APowerupHealth::BeginPlay()
{
	Super::BeginPlay();

}

void APowerupHealth::BeginPowerup()
{
	Super::BeginPowerup();

	if (PlayerTank && PlayerTank->IsAlive())
	{
		if ((PlayerTank->HealthComp->GetHealth() + HealAmount) >= PlayerTank->HealthComp->GetMaxHealth())  // If new health would be greater than max health
		{
			PlayerTank->HealthComp->SetHealth(PlayerTank->HealthComp->GetMaxHealth());                     // Set to max health
		}
		else
		{
			PlayerTank->HealthComp->SetHealth(PlayerTank->HealthComp->GetHealth() + HealAmount);           // Else add heal amount to current health
		}
		UE_LOG(LogTemp, Warning, TEXT("Player Health: %f"), PlayerTank->HealthComp->GetHealth());
	}
}

void APowerupHealth::EndPowerup()
{
	Super::EndPowerup();

}