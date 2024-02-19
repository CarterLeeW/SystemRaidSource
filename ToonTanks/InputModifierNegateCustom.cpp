// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModifierNegateCustom.h"

FInputActionValue UInputModifierNegateCustom::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	return CurrentValue.Get<FVector>() * FVector(bX ? -1.f : 1.f, bY ? -1.f : 1.f, bZ ? -1.f : 1.f);
}


FLinearColor UInputModifierNegateCustom::GetVisualizationColor_Implementation(FInputActionValue SampleValue, FInputActionValue FinalValue) const
{
	FVector Sample = SampleValue.Get<FVector>();
	FVector Final = FinalValue.Get<FVector>();
	return FLinearColor(Sample.X != Final.X ? 1.f : 0.f, Sample.Y != Final.Y ? 1.f : 0.f, Sample.Z != Final.Z ? 1.f : 0.f);
}