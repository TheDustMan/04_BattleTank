// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	ATank* ControlledTank = GetControlledTank();
	if (ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI has possessed pawn: %s"), *ControlledTank->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find AI possessed pawn"));
	}

	ATank* PlayerControlledTank = GetPlayerTank();
	if (PlayerControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI has found player pawn: %s"), *PlayerControlledTank->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find player possessed pawn"));
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPlayerTank())
	{
		// TODO Move towards the player

		// Aim at the player
		AimAtPlayer();

		// Fire at the player
	}
}

void ATankAIController::AimAtPlayer() const
{
	if (GetPlayerTank())
	{
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	ATank* PlayerTank = nullptr;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (PlayerController)
	{
		PlayerTank = Cast<ATank>(PlayerController->GetPawn());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find first player controller"));
	}

	return PlayerTank;
}