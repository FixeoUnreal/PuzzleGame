// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

class ATargetPoint;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();

	void AddActiveTrigger();

	void RemoveActiveTrigger();
	
protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Moving Platform", meta = (MakeEditWidget = true))
	FVector RelativeDestination;

	// Moving speed in cm/s
	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	float Speed;

	bool bInBDirection;

	void MoveBackAndForth(float DeltaSeconds);

private:
	FVector StartWorldLocation;

	FVector TargetWorldLocation;

	// Only move when greater than 0
	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	uint32 ActiveTriggersNr = 1;

	
};
