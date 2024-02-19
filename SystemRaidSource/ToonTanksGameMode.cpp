// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "BasePawn.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "ReactorCore.h"
#include "GameFramework/FloatingPawnMovement.h"


void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTanksGameMode::Tick(float DeltaTime)
{
	ElapsedTime += DeltaTime;
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (ATower* const DeadTower = Cast<ATower>(DeadActor))
	{
		DeadTower->HandleDestruction();
		TargetTowers--;
		TowersDestroyed++;
		TowerDestroyed();
	}
	if (AReactorCore* const DeadReactorCore = Cast<AReactorCore>(DeadActor))
	{
		ReactorCoresCount--;
		if (ReactorCoresCount <= 0)
		{
			GetWorldTimerManager().SetTimer(EscapeTimerHandle, this, &AToonTanksGameMode::EscapeTimerEnd, EscapeTime, false);
			bHasEscapeTimerStarted = true;
			AllReactorsDestroyed();            // Blueprint event
		}
	}
	else if (DeadActor == PlayerTank)
	{
		PlayerTank->HandleDestruction();
		GameOver(false);
	}
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	ReactorCoresCount = GetReactorCoresCount();
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!PlayerTank) UE_LOG(LogTemp, Error, TEXT("PlayerTank is null in ToonTanksGameMode"));

	StartGame();

	if (ToonTanksPlayerController)
	{
		if (bDelayStart)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);

			FTimerHandle PlayerEnableTimerHandle;
			FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
					ToonTanksPlayerController, 
					&AToonTanksPlayerController::SetPlayerEnabledState, 
					true);
			if (StartDelaySec < 1)
			{
				StartDelaySec = 1;  // zero and negative values not compatible.
			}
			GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, (float)StartDelaySec, false);
		}
		else {
			ToonTanksPlayerController->SetPlayerEnabledState(true);
		}
	}
	else UE_LOG(LogTemp, Error, TEXT("ToonTanksPlayerController is null in ToonTanksGameMode"));
}

int32 AToonTanksGameMode::GetTargetTowerCount() const
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
	return Towers.Num();
}

int32 AToonTanksGameMode::GetReactorCoresCount() const
{
	TArray<AActor*> ReactorCores;
	UGameplayStatics::GetAllActorsOfClass(this, AReactorCore::StaticClass(), ReactorCores);
	return ReactorCores.Num();
}

void AToonTanksGameMode::AddCoins(int32 CoinsToAdd)
{
	CoinsCollected += CoinsToAdd;
	PickupCoins();
}

void AToonTanksGameMode::EscapeTimerEnd()
{
	PlayerTank->HandleDestruction();
	GameOver(false);
}

void AToonTanksGameMode::PlayerEscaped()
{
	GetWorldTimerManager().ClearTimer(EscapeTimerHandle);
	PlayerTank->GetTankPlayerController()->SetPlayerEnabledState(false);

	// Score calculation
	KillScore = TowersDestroyed * 60;
	CoinScore = CoinsCollected * 200;
	if (ElapsedTime > 0.0f)
	{
		TimeScore = (int32)(1000000.0 / ElapsedTime);
	}
	else
	{
		TimeScore = 0;
	}
	TotalScore = KillScore + CoinScore + TimeScore;

	GameOver(true);
}