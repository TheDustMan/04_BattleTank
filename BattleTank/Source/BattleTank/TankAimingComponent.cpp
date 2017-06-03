// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
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

void UTankAimingComponent::AimAt(FVector Location, float LaunchSpeed)
{
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
		ESuggestProjVelocityTraceOption::TraceFullPath,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		false);

	if (SuccessfulSuggestion)
	{
		auto AimDirection = TossVelocity.GetSafeNormal();
		UE_LOG(LogTemp, Warning, TEXT("Firing at launchspeed %s in direction %s"), *FString::SanitizeFloat(LaunchSpeed), *AimDirection.ToString());
		MoveBarrel(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrel(FVector AimDirection)
{
	if (Barrel)
	{
		// Adjust the Barrel's pitch according to the pitch derived from the AimDirection
		FRotator CurrentBarrelRotation = Barrel->GetForwardVector().Rotation();
		UE_LOG(LogTemp, Warning, TEXT("CurrentBarrelRotation is %s"), *CurrentBarrelRotation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("New rotation is %s"), *AimDirection.Rotation().ToString());
		//FRotator NewBarrelRotation = FMath::RInterpTo(CurrentBarrelRotation, AimDirection.Rotation(), GetWorld()->GetDeltaSeconds(), 10.f);
		//FRotator NewBarrelPitch(NewBarrelRotation.Pitch, 0.f, 0.f);
		Barrel->SetWorldRotation(CurrentBarrelRotation);
		Barrel->Elevate(5);
	}
	/*
	if (Turret)
	{
		// Adjust the Turret's yaw according to the yaw derived from the AimDirection
		float YawAngle = FMath::Atan2(AimDirection.Y, AimDirection.X);
		FRotator YawRotation = FRotator(0, YawAngle * PI, 0);
		Turret->SetWorldRotation(YawRotation);
		UE_LOG(LogTemp, Warning, TEXT("Turret %s"), *FString::SanitizeFloat(YawAngle));
		Turret->Pivot(5);
	}
	*/
}