// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UGameStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Tick(FGeometry MyGeometry, float InDeltaTime);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void CallOnTick();
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayText = "Default Text";
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	float Countdown = 2.0f;

	class AToonTanksGameMode* GameMode;
};
