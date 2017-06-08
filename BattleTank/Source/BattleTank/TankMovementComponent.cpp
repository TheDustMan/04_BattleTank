// Copyright DustIndustries

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
	if (!ensure(LeftTrack))
	{
		UE_LOG(LogTemp, Error, TEXT("NULL reference to LeftTrack"));
		return;
	}
	if (!ensure(RightTrack))
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
	if (!ensure(LeftTrack))
	{
		UE_LOG(LogTemp, Error, TEXT("NULL reference to LeftTrack"));
		return;
	}
	if (!ensure(RightTrack))
	{
		UE_LOG(LogTemp, Error, TEXT("NULL reference to RightTrack"));
		return;
	}

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	auto TankForward = GetOwner()->GetActorForwardVector();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	float DotProduct = FVector::DotProduct(TankForward, AIForwardIntention);
	FVector CrossProduct = FVector::CrossProduct(TankForward, AIForwardIntention);
	IntendTurnRight(CrossProduct.Z);
	IntendMoveForward(DotProduct);
}