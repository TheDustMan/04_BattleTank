// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"
#include "Tank.h"
#include "TankAimingComponent.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto AimingComponent = GetControlledTankAimingComponent();
	if (ensure(AimingComponent))
	{
		FoundAimingComponent(AimingComponent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller can't find aiming component at BeginPlay"));
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimAtCrossHair();
}

UTankAimingComponent* ATankPlayerController::GetControlledTankAimingComponent() const
{
	ATank* Tank = Cast<ATank>(GetPawn());
	if (!ensure(Tank))
	{
		return nullptr;
	}

	return Tank->FindComponentByClass<UTankAimingComponent>();
}

void ATankPlayerController::AimAtCrossHair()
{
	if (!ensure(GetControlledTankAimingComponent()))
	{
		return;
	}

	FVector HitLocation;
	// Get world location via line trace through crosshair
	// If it hits the landscape
	if (GetSightRayHitLocation(HitLocation))
	{
		// Tell the controlled tank to aim toward the crosshair
		GetControlledTankAimingComponent()->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

	// De-project the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// Line trace along that look direction, and see what we hit (up to a max range)
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}

	return  false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& OutLookDirection) const
{
	FVector OutCameraWorldLocation;
	FVector OutWorldDirection;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, OutCameraWorldLocation, OutLookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const
{
	FHitResult HitResult;
	FVector StartLocation = PlayerCameraManager->GetCameraLocation(); // Where the camera is at
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);

	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.f, 0, 5.f);
	bool HitDetected = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility);
	if (HitDetected)
	{
		OutHitLocation = HitResult.Location;
	}
	else
	{
		OutHitLocation = FVector(0.f);
	}

	return HitDetected;
}