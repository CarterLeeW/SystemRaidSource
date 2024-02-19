// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorCore.h"
#include "ExplodingComponent.h"
#include "HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AReactorCore::AReactorCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	RootComponent = RootSceneComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	ExplodingComp = CreateDefaultSubobject<UExplodingComponent>(TEXT("Exploding"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

// Called when the game starts or when spawned
void AReactorCore::BeginPlay()
{
	Super::BeginPlay();

	MeshMaterial = BaseMesh->CreateDynamicMaterialInstance(0);
	if (MeshMaterial)
	{
		MeshMaterial->SetVectorParameterValue("Colour", StartingColor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshMaterial is null in ReactorCore"));
	}

	OnTakeAnyDamage.AddDynamic(this, &AReactorCore::DamageTaken);
	
}

// Called every frame
void AReactorCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthComp->GetHealth() <= 0.0f)
	{
		ExplodingComp->Explode();
	}
}

void AReactorCore::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UpdateColorOnDamage();
}

void AReactorCore::UpdateColorOnDamage()
{
	float ColorAlpha = HealthComp->GetHealth() / HealthComp->GetMaxHealth();
	FLinearColor NewColor = UKismetMathLibrary::LinearColorLerp(EndingColor, StartingColor, ColorAlpha);
	MeshMaterial->SetVectorParameterValue("Colour", NewColor);
}
