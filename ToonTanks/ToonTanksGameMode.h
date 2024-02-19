// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	void ActorDied(AActor* DeadActor);

	UFUNCTION(BlueprintCallable)
	int GetStartDelaySec() const { return StartDelaySec; }

	UFUNCTION(BlueprintCallable)
	bool ShouldDelayStart() const { return bDelayStart; }

	// Coins
	UFUNCTION(BlueprintCallable)
	void AddCoins(int32 CoinsToAdd);
	UFUNCTION(BlueprintImplementableEvent)
	void PickupCoins();
	UFUNCTION(BlueprintImplementableEvent)
	void TowerDestroyed();
	
	// Escape timer
	UFUNCTION(BlueprintCallable)
	bool HasEscapeTimerStarted() const { return bHasEscapeTimerStarted; }
	UFUNCTION(BlueprintCallable)
	void PlayerEscaped();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int StartDelaySec = 3;

	UPROPERTY(EditAnywhere)
	bool bDelayStart = true;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	UFUNCTION(BlueprintImplementableEvent)
	void AllReactorsDestroyed();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CoinsCollected = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TowersDestroyed = 0;

	UPROPERTY(BlueprintReadWrite)
	float ElapsedTime = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float EscapeTime = 60.0f;

	// Score
	UPROPERTY(BlueprintReadOnly)
	int32 KillScore = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 CoinScore = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 TimeScore = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 TotalScore = 0;
private:
	class AToonTanksPlayerController* ToonTanksPlayerController;
	class ATank* PlayerTank;

	void HandleGameStart();
	int32 GetTargetTowerCount() const;
	int32 TargetTowers = 0;
	int32 ReactorCoresCount = 0;
	int32 GetReactorCoresCount() const;

	// Escape timer
	void EscapeTimerEnd();
	FTimerHandle EscapeTimerHandle;
	bool bHasEscapeTimerStarted = false;
};
