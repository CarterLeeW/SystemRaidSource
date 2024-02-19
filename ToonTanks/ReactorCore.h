// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReactorCore.generated.h"

class UHealthComponent;
class UExplodingComponent;

UCLASS()
class TOONTANKS_API AReactorCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReactorCore();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UExplodingComponent* ExplodingComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;

	// Material and color
	UMaterialInstanceDynamic* MeshMaterial;
	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor StartingColor = FLinearColor::Green;
	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor EndingColor = FLinearColor::Red;
private:
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	void UpdateColorOnDamage();
};
