// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class IMenuInterface;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	virtual void Setup();

	virtual void TearDown();
	
protected:
	IMenuInterface* MenuInterface;
	
};
