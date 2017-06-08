// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankAimingComponent.h"
#include "Projectile.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; // TODO Should this really tick?

	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::AimAt(FVector Location)
{
	if (!ensure(Barrel))
	{
		return;
	}

	FVector TossVelocity;
	bool SuccessfulSuggestion = UGameplayStatics::SuggestProjectileVelocity(
		GetWorld(),
		TossVelocity,
		Barrel->GetSocketLocation(FName("Projectile")),
		Location,
		LaunchSpeed,
		false,
		0.f, // TODO Make parameterizable
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace, // We must use DoNotTrace or or we won't get a solution all the time (bug in UnrealEngine)
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		false);

	if (SuccessfulSuggestion)
	{
		auto AimDirection = TossVelocity.GetSafeNormal();
		//UE_LOG(LogTemp, Warning, TEXT("%f: Aim solution found, firing at launchspeed %f in direction %s"), GetWorld()->GetTimeSeconds(), LaunchSpeed, *AimDirection.ToString());
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (ensure(Barrel))
	{
		// Adjust the Barrel's pitch according to the pitch derived from the AimDirection
		auto CurrentBarrelRotation = Barrel->GetForwardVector().Rotation();
		auto AimAtRotation = AimDirection.Rotation();
		auto DeltaRotation = AimAtRotation - CurrentBarrelRotation;
		
		//UE_LOG(LogTemp, Warning, TEXT("Current Pitch: %f, AimPitch: %f, ElevateDir: %f"), CurrentBarrelPitch, AimDirection.Rotation().Pitch, ElevateDirection);
		Barrel->Elevate(DeltaRotation.Pitch);
	}
	if (ensure(Turret))
	{
		// Adjust the Turret's yaw according to the yaw derived from the AimDirection
		auto CurrentTurretRotation = Turret->GetForwardVector().Rotation();
		auto AimAtRotation = AimDirection.Rotation();
		auto DeltaRotation = AimAtRotation - CurrentTurretRotation;
		Turret->Pivot(DeltaRotation.Yaw);
	}
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel))
	{
		return;
	}

	bool IsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (IsReloaded)
	{
		// Spawn a projectile at the socket location
		FVector FireLocation = Barrel->GetSocketLocation(FName("Projectile"));
		FRotator FireRotation = Barrel->GetSocketRotation(FName("Projectile"));;
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, FireLocation, FireRotation);
		if (ensure(Projectile))
		{
			Projectile->LaunchProjectile(LaunchSpeed);
			LastFireTime = FPlatformTime::Seconds();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%f: Unable to spawn projectile"), GetWorld()->GetTimeSeconds());
		}
	}
}