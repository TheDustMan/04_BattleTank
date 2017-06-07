// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!LeftTrack)
	{
		UE_LOG(LogTemp, Error, TEXT("NULL reference to LeftTrack"));
		return;
	}
	if (!RightTrack)
	{
		UE_LOG(LogTemp, Error, TEXT("NULL reference to RightTrack"));
		return;
	}

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

	// TODO prevent double-speed due to dual control use
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!LeftTrack)
	{
		UE_LOG(LogTemp, Error, TEXT("NULL reference to LeftTrack"));
		return;
	}
	if (!RightTrack)
	{
		UE_LOG(LogTemp, Error, TEXT("NULL reference to RightTrack"));
		return;
	}

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}


