// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include <Engine/TargetPoint.h>



AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

	Speed = 5.f;
	bInBDirection = true;
	RelativeDestination = FVector(500, 0, 0);
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		MoveBetweenAB(DeltaSeconds);
	}


}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	PointA = GetActorLocation();
	PointB = GetActorLocation() + RelativeDestination;

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::MoveBetweenAB(float DeltaSeconds)
{
	FVector ToBVelocity = ((PointB - PointA) / (PointB - PointA).Size()) * DeltaSeconds * Speed;
	if (bInBDirection)
	{
		if ((GetActorLocation() - PointB).Size() > 50)
		{
			SetActorLocation(GetActorLocation() + ToBVelocity);
		}
		else
		{
			bInBDirection = false;
		}
	}
	else
	{
		if ((GetActorLocation() - PointA).Size() > 50)
		{
			SetActorLocation(GetActorLocation() - ToBVelocity);
		}
		else
		{
			bInBDirection = true;
		}
	}
}
