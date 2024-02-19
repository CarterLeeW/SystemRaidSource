// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TimerManager.h"
#include "PowerupBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabberComponent.h"

// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;
	// Components
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	BaseHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Base Hit Box"));
	BaseHitBox->SetupAttachment(RootComponent);

	TankCenter = CreateDefaultSubobject<USceneComponent>(TEXT("Tank Center Point"));
	TankCenter->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(RootComponent);

	TurretHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Turret Hit Box"));
	TurretHitBox->SetupAttachment(TurretMesh);

	TurretCenter = CreateDefaultSubobject<USceneComponent>(TEXT("Turret Center Point"));
	TurretCenter->SetupAttachment(TurretMesh);


	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	GrabberComp = CreateDefaultSubobject<UGrabberComponent>(TEXT("Grabber"));
	GrabberComp->SetupAttachment(SpringArmComp);

	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	// Overlap delegates
	OnActorBeginOverlap.AddDynamic(this, &ATank::OnOverlapBegin);

	if (CannonFireRate <= 0.0f)
	{
		CannonFireRate = 0.1f;
	}

	// Editor overwritten variables
	// Spring Arm
	SpringArmRegularLength = SpringArmComp->TargetArmLength;
	AimSensitivityCurrent = AimSensitivityDefault;

	// Get player controller
	TankPlayerController = Cast<AToonTanksPlayerController>(GetController());
	if (TankPlayerController)
	{
		// Get local player subsystem	
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TankPlayerController->GetLocalPlayer()))
		{
			// Clear out existing mapping, and add our mapping
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TankPlayerController is nullptr"));
	}
	bIsAlive = true;

	GetFloatingPawnMovement();
	if (!FloatingPawnMovementComp)
	{
		UE_LOG(LogTemp, Error, TEXT("FloatingPawnMovementComp must be set in Tank Blueprint using EventGetFloatingPawnMovement"));
	}
}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTurretToController();
	CorrectTankRotation();

	// line trace here for crosshair using camera channel
}

/** Only One powerup may be active on the player at any time
    If a different powerup (a) is consumed while another is active (b)
	The consumed powerup (a) will become active and the other powerup (b)
	will be removed and its respective EndPowerup function will be called*/
void ATank::ConsumePowerup(APowerupBase* NewPowerup)
{
	FTimerDelegate PowerupDurationDelegate = FTimerDelegate::CreateUObject(
			this,
			&ATank::RemovePowerup,
			NewPowerup
	);

	if (GetWorldTimerManager().IsTimerActive(PowerupDurationHandle) && ActivePowerup && NewPowerup->IsAvailable() && (NewPowerup != ActivePowerup))
	{
		// Remove current powerup and clean up its effects
		GetWorldTimerManager().ClearTimer(PowerupDurationHandle);
		ActivePowerup->EndPowerup();

		// Begin new powerup
		GetWorldTimerManager().SetTimer(PowerupDurationHandle, PowerupDurationDelegate, NewPowerup->GetDuration(), false);
		NewPowerup->BeginPowerup();
		ActivePowerup = NewPowerup;
	}
	else if (NewPowerup->IsAvailable())
	{
		// Begin new powerup
		GetWorldTimerManager().SetTimer(PowerupDurationHandle, PowerupDurationDelegate, NewPowerup->GetDuration(), false);
		NewPowerup->BeginPowerup();
		ActivePowerup = NewPowerup;
	}
}
/** Removes currently active powerup by callings its EndPowerup function
    and sets ActivePowerup to nullptr
	May be called by the timer delegate or immediately if old powerup is to be
	replaced by a new one*/
void ATank::RemovePowerup(APowerupBase* OldPowerup)
{
	OldPowerup->EndPowerup();
	if (ActivePowerup)
	{
		ActivePowerup = nullptr;
	}

}

void ATank::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped %s"), *OtherActor->GetName());
	// Powerup Handling
	if (APowerupBase* Powerup = Cast<APowerupBase>(OtherActor))
	{
		ConsumePowerup(Powerup);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	TankPlayerController->SetPlayerEnabledState(false);
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bIsAlive = false;
}

AToonTanksPlayerController* ATank::GetTankPlayerController() const
{
	return TankPlayerController;
}

/** Deprecated */
void ATank::LerpTurretRotation()
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FRotator LookAtRotation = TankPlayerController->GetControlRotation();

	UE_LOG(LogTemp, Display, TEXT("Pitch: %f"), LookAtRotation.Pitch);

	if ((LookAtRotation.Pitch > MaxTurretPitch) && (LookAtRotation.Pitch < 270))
	{
		LookAtRotation.Pitch = MaxTurretPitch;
	}
	else if ((LookAtRotation.Pitch < 360.0 - MaxTurretPitch) && (LookAtRotation.Pitch >=270))
	{
		LookAtRotation.Pitch = -MaxTurretPitch;
	}
	FRotator NewRotation = FRotator(LookAtRotation.Pitch, LookAtRotation.Yaw, 0.0);
	TurretMesh->SetWorldRotation(FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			NewRotation,
			DeltaTime,
			TurretRotateLerp));
}

void ATank::RotateTurretToController()
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	// Allows for line trace to be from the controller's point of view, while the turret
	// will point to the impact point, which makes aiming more fluid and comfortable.
	FVector ControllerForwardVector = UKismetMathLibrary::CreateVectorFromYawPitch(TankPlayerController->GetControlRotation().Yaw, TankPlayerController->GetControlRotation().Pitch);
	FVector Start = SpringArmComp->GetComponentLocation();
	FVector End = Start + (ControllerForwardVector * 10000.0);   // variable?

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility
	);

	FRotator TargetRotation;
	if (HitResult.bBlockingHit)
	{
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretCenter->GetComponentLocation(), HitResult.ImpactPoint);
		CrosshairTraceEnd = HitResult.ImpactPoint;
	}
	else {
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretCenter->GetComponentLocation(), HitResult.TraceEnd);
		CrosshairTraceEnd = HitResult.TraceEnd;
	}

	// Band-aid fix for turret spasm when looking down or getting too close to wall. Still has minor problems looking down.
	if (HitResult.bBlockingHit && (FVector::Distance(TurretCenter->GetComponentLocation(), HitResult.ImpactPoint) <= 200.0))
	{
		TargetRotation.Pitch = 0.0;
	}
	/* Is this unnecessary?
	if ((TargetRotation.Pitch > MaxTurretPitch) && (TargetRotation.Pitch < 270.0))
	{
		TargetRotation.Pitch = MaxTurretPitch;
	}
	else if ((TargetRotation.Pitch < 360.0 - MaxTurretPitch) && (TargetRotation.Pitch >= 270.0))
	{
		TargetRotation.Pitch = -MaxTurretPitch;
	}
	*/

	float PitchLerp = FMath::FInterpTo(TurretMesh->GetComponentRotation().Pitch, TargetRotation.Pitch, DeltaTime, TurretRotateLerp);
	// These ControlRotation values are always consistent and help prevent spasming
	TargetRotation.Yaw = GetControlRotation().Yaw;
	TargetRotation.Roll = GetControlRotation().Roll;

	TurretMesh->SetWorldRotation(FRotator(PitchLerp, TargetRotation.Yaw, TargetRotation.Roll));

	FRotator RelativeRotation = TurretMesh->GetRelativeRotation();
	// Check relative rotation
	if (RelativeRotation.Pitch > MaxTurretPitch)
	{
		RelativeRotation.Pitch = MaxTurretPitch;
	}
	else if (RelativeRotation.Pitch < -MaxTurretPitch)
	{
		RelativeRotation.Pitch = -MaxTurretPitch;
	}
	if (RelativeRotation.Roll > MaxTurretRoll)
	{
		RelativeRotation.Roll = MaxTurretRoll;
	}
	else if (RelativeRotation.Roll < -MaxTurretRoll)
	{
		RelativeRotation.Roll = -MaxTurretRoll;
	}

	TurretMesh->SetRelativeRotation(RelativeRotation);
}

/** Deprecated */
void ATank::SnapTurretRotation()
{

	FRotator TargetRotation = TankPlayerController->GetControlRotation();
	
	// Check world rotation
	if ((TargetRotation.Pitch > MaxTurretPitch) && (TargetRotation.Pitch < 270))
	{
		TargetRotation.Pitch = MaxTurretPitch;
	}
	else if ((TargetRotation.Pitch < 360.0 - MaxTurretPitch) && (TargetRotation.Pitch >= 270))
	{
		TargetRotation.Pitch = -MaxTurretPitch;
	}
	TurretMesh->SetWorldRotation(TargetRotation);

	FRotator RelativeRotation = TurretMesh->GetRelativeRotation();
	// Check relative rotation
	if (RelativeRotation.Pitch > MaxTurretPitch)
	{
		RelativeRotation.Pitch = MaxTurretPitch;
	}
	else if (RelativeRotation.Pitch < -MaxTurretPitch)
	{
		RelativeRotation.Pitch = -MaxTurretPitch;
	}
	if (RelativeRotation.Roll > MaxTurretRoll)
	{
		RelativeRotation.Roll = MaxTurretRoll;
	}
	else if (RelativeRotation.Roll < -MaxTurretRoll)
	{
		RelativeRotation.Roll = -MaxTurretRoll;
	}
	TurretMesh->SetRelativeRotation(RelativeRotation);
}

void ATank::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	if (DirectionValue != 0.0)
	{
		AddMovementInput(GetActorForwardVector(), DirectionValue);		
	}
}

void ATank::Pivot(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	if (DirectionValue != 0.0)
	{
		AddActorLocalRotation(FRotator(0.0, DirectionValue * PivotConstant * DeltaTime, 0.0), false);
	}
}

void ATank::AimTurret(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	if (DirectionValue.X != 0.0)
	{
		AddControllerYawInput(DirectionValue.X * AimSensitivityCurrent * DeltaTime);

	}
	//GetController()->GetControlRotation
	if (DirectionValue.Y != 0.00)
	{
		AddControllerPitchInput(DirectionValue.Y * AimSensitivityCurrent * DeltaTime);
	}
}

void ATank::Fire()
{
	switch (ActiveAmmoType)
	{
	case EAmmoType::CANNON:
		if (bCanFireCannon)
		{
			GetWorldTimerManager().SetTimer(CannonTimerHandle, this, &ATank::RefreshCannon, CannonFireRate, false);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();               // is this necessary?
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
					CannonProjectileClass,
					ProjectileSpawnPoint->GetComponentLocation(),
					ProjectileSpawnPoint->GetComponentRotation(),
					SpawnParams);
			bCanFireCannon = false;
		}
		break;
	case EAmmoType::MACHINE_GUN:
		if (bCanFireMachineGun)
		{
			GetWorldTimerManager().SetTimer(MachineGunTimerHandle, this, &ATank::RefreshMachineGun, MachineGunFireRate, false);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();               // is this necessary?
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
					MachineGunProjectileClass,
					ProjectileSpawnPoint->GetComponentLocation(),
					ProjectileSpawnPoint->GetComponentRotation() + FRotator(
							FMath::RandRange(-MachineGunSpread, MachineGunSpread), 
							FMath::RandRange(-MachineGunSpread, MachineGunSpread), 
							FMath::RandRange(-MachineGunSpread, MachineGunSpread)),
					SpawnParams);
			bCanFireMachineGun = false;
		}
		break;
	case EAmmoType::GRABBER:
		if (bIsGrabbing)
		{
			Throw();

		}
		break;
	default:
		break;
	}
}

void ATank::SwapAmmoType()
{
	switch (ActiveAmmoType)
	{
	case EAmmoType::CANNON:
		ActiveAmmoType = EAmmoType::MACHINE_GUN;
		if (SwitchToMachineGunSound)
		{
			UGameplayStatics::PlaySound2D(this, SwitchToMachineGunSound);
		}
		break;
	case EAmmoType::MACHINE_GUN:
		ActiveAmmoType = EAmmoType::CANNON;
		if (SwitchToCannonSound)
		{
			UGameplayStatics::PlaySound2D(this, SwitchToCannonSound);
		}
		break;
	default:
		ActiveAmmoType = EAmmoType::CANNON;
		break;
	}
}

void ATank::Grab()
{
	if (bCanGrab)
	{
		
		if (!bIsGrabbing)  // Remember last active ammo type
		{
			LastActiveAmmoType = ActiveAmmoType;
		}
		bIsGrabbing = true;
		GrabberComp->Grab();
		if (UPrimitiveComponent* GrabbedComponent = PhysicsHandleComp->GetGrabbedComponent())
		{
			GrabbedComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		}
		ActiveAmmoType = EAmmoType::GRABBER;
	}
}

void ATank::Release()
{
	ActiveAmmoType = LastActiveAmmoType;
	bIsGrabbing = false;
	if (UPrimitiveComponent* GrabbedComponent = PhysicsHandleComp->GetGrabbedComponent())
	{
		GrabbedComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	}
	GrabberComp->Release();
	bCanGrab = true;
}

void ATank::Throw()
{
	bCanGrab = false;
	if (UPrimitiveComponent* GrabbedComponent = PhysicsHandleComp->GetGrabbedComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Throwing"));
		GrabberComp->Release();
		UE_LOG(LogTemp, Warning, TEXT("Grabbed Component %s"), *GrabbedComponent->GetName());
		GrabbedComponent->GetOwner()->Tags.Add("thrown");
		GrabbedComponent->bApplyImpulseOnDamage = false;
		GrabbedComponent->AddImpulse((GrabberComp->GetForwardVector() * ThrowStrength) + FVector(0.0, 0.0, 500.0), NAME_None, true); // TODO: Make Z coord relative to crosshair trace end
	}
	GetWorldTimerManager().SetTimer(CanGrabTimerHandle, this, &ATank::Release, 0.5f, false);
}

// Unused - Implemented in blueprints instead
void ATank::Zoom(const FInputActionValue& Value)
{
	const bool bShouldZoom = Value.Get<bool>();
	if (bShouldZoom)
	{
		UE_LOG(LogTemp, Display, TEXT("zoom in"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("zoom out"));
	}
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(PivotAction, ETriggerEvent::Triggered, this, &ATank::Pivot);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ATank::AimTurret);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATank::Fire);
		EnhancedInputComponent->BindAction(SwapAmmoTypeAction, ETriggerEvent::Triggered, this, &ATank::SwapAmmoType);
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Triggered, this, &ATank::Grab);
		EnhancedInputComponent->BindAction(ReleaseAction, ETriggerEvent::Triggered, this, &ATank::Release);
		//EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATank::Zoom);
	}

}

void ATank::CorrectTankRotation()
{
	if (GetActorRotation().Roll > MaxTankRoll)
	{
		SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, MaxTankRoll));
	}
	else if (GetActorRotation().Roll < -MaxTankRoll)
	{
		SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, -MaxTankRoll));
	}
	if (GetActorRotation().Pitch > MaxTankPitch)
	{
		SetActorRotation(FRotator(MaxTankPitch, GetActorRotation().Yaw, GetActorRotation().Roll));
	}
	else if (GetActorRotation().Pitch < -MaxTankPitch)
	{
		SetActorRotation(FRotator(-MaxTankPitch, GetActorRotation().Yaw, GetActorRotation().Roll));
	}
}