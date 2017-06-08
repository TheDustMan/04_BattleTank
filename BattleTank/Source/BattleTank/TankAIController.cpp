// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAIController.h"
#include "Tank.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATank* ControlledTank = Cast<ATank>(GetPawn());
	ATank* PlayerTank = nullptr;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerTank = Cast<ATank>(PlayerController->GetPawn());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to acces PlayerController, PlayerTank will be NULL"));
	}
	

	if (PlayerTank && ControlledTank)
	{
		// Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim at the player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		// Fire at the player
		ControlledTank->Fire();
	}
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL reference to PlayerControlled Tank"));
	}
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL reference to AIControlled Tank"));
	}
}
