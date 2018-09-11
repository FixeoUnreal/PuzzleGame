// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;
class IOnlineSubsystem;


/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init() override;

	UFUNCTION(Exec)
	virtual void Host() override;

	UFUNCTION(Exec)
	virtual void Join(const FString& Address) override;

	UFUNCTION(Exec)
	void OpenMainMenu() override;

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void LoadMenuWidget(TSubclassOf<UMenuWidget> MenuClass);

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void ExitMenu();

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Success);

	void StartHostSession(FName SessionName);

	void OnDestroySessionCompleted(FName SessionName, bool Success);

	void CreateSession();


private:
	TSubclassOf<UUserWidget> MainMenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;

	UMenuWidget* CurrentMenu;

	IOnlineSessionPtr SessionInterface;
};
