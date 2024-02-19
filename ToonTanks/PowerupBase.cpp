// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupBase.h"
#include "TriggerComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "TimerManager.h"

// Sets default values
APowerupBase::APowerupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Component"));
	RootComponent = DefaultSceneRoot;

	TriggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("Trigger Box Comp"));
	TriggerComp->SetupAttachment(RootComponent);

	NiagaraEffects = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Effects"));
	NiagaraEffects->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APowerupBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerTank is null in PowerupBase"));
	}

	if (EffectDuration < 1.0f)
	{
		EffectDuration = 1.0f;
		UE_LOG(LogTemp, Warning, TEXT("Powerup EffectDuration must be greater than or equal to 1.0"));
	}
}

// Called every frame
void APowerupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerupBase::BeginPowerup()
{
	bIsAvailable = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	if (bShouldRespawn)
	{
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APowerupBase::RespawnPowerup, RespawnAfterSec, false);
	}
}

void APowerupBase::EndPowerup()
{
	
}

void APowerupBase::RespawnPowerup()
{
	bIsAvailable = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}