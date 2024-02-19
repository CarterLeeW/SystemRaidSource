// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingComponent.h"

// Sets default values for this component's properties
UMovingComponent::UMovingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMovingComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	
}


// This can be moved to an event to be more efficient
void UMovingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldMove)
	{
		TargetLocation = OriginalLocation + MoveOffset;
		FVector CurrentLocation = GetOwner()->GetActorLocation();

		float Speed = MoveOffset.Length() / MoveTime;

		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(NewLocation);
		if (NewLocation == TargetLocation)
		{
			bShouldMove = false;
		}
	}
}
