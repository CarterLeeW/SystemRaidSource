// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }
	void SetHealth(float NewHealth) { Health = NewHealth; }
	UFUNCTION(BlueprintCallable)
	bool CanTakeDamage() const { return bCanTakeDamage; }
	void SetCanTakeDamage(bool NewCanTakeDamage) { bCanTakeDamage = NewCanTakeDamage; }
private:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;
	float Health = 0.0f;
	bool bCanTakeDamage = true;

	class AToonTanksGameMode* ToonTanksGameMode;

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);
};
