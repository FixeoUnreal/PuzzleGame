// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class IMenuInterface;
class ULevel;
class UWorld;
class UWidgetSwitcher;
class UEditableTextBox;
class UInputComponent;
class UWidget;
class UUserWidget;
class FObjectInitializer;
class FOnlineSessionSearchResult;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	void SetServerList(TArray<FOnlineSessionSearchResult> ServerList);

	void SelectIndex(uint32 Index);

	void JoinSelectedServer();

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
	UButton* BtnQuit;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerScrollBox;

	TOptional<uint32> SelectedIndex;

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

	UFUNCTION()
	void QuitClicked();

private:
	TSubclassOf<UUserWidget> ServerRowClass;

private:
	void AddServerRow(const FString& DisplayStr, uint32 Index);

	void UpdateChildren();
};
