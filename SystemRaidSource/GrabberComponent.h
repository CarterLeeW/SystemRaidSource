// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GrabberComponent.generated.h"

class UPhysicsHandleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UGrabberComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Release();
	UFUNCTION(BlueprintCallable)
	void Grab();

protected:
	UPhysicsHandleComponent* PhysicsHandle;
	UPhysicsHandleComponent* GetPhysicsHandle() const;
private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400.0;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 100.0;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 200.0;


	bool GetGrabbableInReach(FHitResult& OutHitResult) const;
};
