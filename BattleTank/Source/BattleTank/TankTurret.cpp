// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankTurret.h"

void UTankTurret::Pivot(float DegreesPerSecond)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret pivot called with speed %s"), *FString::SanitizeFloat(DegreesPerSecond));
}