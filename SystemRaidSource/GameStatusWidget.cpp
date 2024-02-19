// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStatusWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameMode.h"

void UGameStatusWidget::Tick(FGeometry MyGeometry, float InDeltaTime)
{
	DisplayText = FString::SanitizeFloat(5.0f);
	Countdown++;
}

void UGameStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DisplayText = FString::SanitizeFloat(5.0f);
	Countdown++;
}

void UGameStatusWidget::CallOnTick()
{
	DisplayText = "Tick?";
}

void UGameStatusWidget::NativeConstruct()
{
	DisplayText = "Setting in Construct";
	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		Countdown = GameMode->GetStartDelaySec();
		if (Countdown > 2)
		{

		}
	}
}


