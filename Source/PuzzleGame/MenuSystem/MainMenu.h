// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
class IMenuInterface;
class ULevel;
class UWorld;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	void Setup();

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BtnHost;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BtnJoin;

	IMenuInterface* MenuInterface;

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void HostClicked();

	UFUNCTION()
	void JoinClicked();
};
