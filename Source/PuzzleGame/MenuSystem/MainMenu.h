// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
class IMenuInterface;
class ULevel;
class UWorld;
class UWidgetSwitcher;
class UEditableTextBox;
class UInputComponent;

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

	UPROPERTY(meta = (BindWidget))
	UButton* BtnJoinWithAddr;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnBack;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IPAddressTextBox;

	IMenuInterface* MenuInterface;

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void HostClicked();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void JoinClickedWithAddress();

	UFUNCTION()
	void BackToMainMenu();
};
