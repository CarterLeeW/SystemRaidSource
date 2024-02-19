// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinPickup.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameMode.h"
#include "NiagaraSystem.h"

// Sets default values
ACoinPickup::ACoinPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Component"));
	RootComponent = DefaultSceneRoot;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Comp"));
	BoxCollision->SetupAttachment(RootComponent);

	NiagaraEffects = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Effects"));
	NiagaraEffects->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACoinPickup::BeginPlay()
{
	Super::BeginPlay();
	
	// Overlap delegates
	OnActorBeginOverlap.AddDynamic(this, &ACoinPickup::OnOverlapBegin);

	ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	
}

// Called every frame
void ACoinPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoinPickup::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		ToonTanksGameMode->AddCoins(CoinValue);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PickupEffect, GetActorLocation());
		NiagaraEffects->SetHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}