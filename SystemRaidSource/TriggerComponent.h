// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;
	
private:


};
