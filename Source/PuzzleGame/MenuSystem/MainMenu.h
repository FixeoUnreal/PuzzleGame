// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BtnHost;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BtnJoin;
};
