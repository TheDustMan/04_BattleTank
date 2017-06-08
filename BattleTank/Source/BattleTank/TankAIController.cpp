// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAIController.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UTankAimingComponent* ControlledTankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	APawn* PlayerTank = nullptr;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (ensure(PlayerController))
	{
		PlayerTank = PlayerController->GetPawn();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to acces PlayerController, PlayerTank will be NULL"));
	}
	

	if (ensure(PlayerTank && ControlledTankAimingComponent))
	{
		// Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim at the player
		ControlledTankAimingComponent->AimAt(PlayerTank->GetActorLocation());

		// Fire at the player
		ControlledTankAimingComponent->Fire();
	}
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL reference to PlayerControlled Tank"));
	}
	if (!ControlledTankAimingComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL reference to AIControlled Tank"));
	}
}
