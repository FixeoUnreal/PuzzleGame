// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include <Engine/TargetPoint.h>



AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

	Speed = 5.f;
	bInBDirection = true;
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggersNr++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if(ActiveTriggersNr <= 0){ return; }
	ActiveTriggersNr--;
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority() && ActiveTriggersNr > 0)
	{
		MoveBackAndForth(DeltaSeconds);
	}


}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartWorldLocation = GetActorLocation();
	TargetWorldLocation = StartWorldLocation + RelativeDestination;

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::MoveBackAndForth(float DeltaSeconds)
{
	FVector ToTargetVelocityPerFrame = (TargetWorldLocation - StartWorldLocation).GetSafeNormal() * Speed * DeltaSeconds;

	if ((GetActorLocation() - StartWorldLocation).Size() < (TargetWorldLocation - StartWorldLocation).Size())
	{
		SetActorLocation(GetActorLocation() + ToTargetVelocityPerFrame);
	}
	else
	{	
		FVector TempVector = TargetWorldLocation;
		TargetWorldLocation = StartWorldLocation;
		StartWorldLocation = TempVector;
	}
}