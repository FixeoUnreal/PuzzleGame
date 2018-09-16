// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include <OnlineSessionInterface.h>
#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;
class IOnlineSubsystem;
class FOnlineSessionSearch;


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
	virtual void Host(const FString& Name) override;

	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;

	UFUNCTION(Exec)
	void OpenMainMenu() override;

	virtual void RefreshServerList() override;

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void LoadMenuWidget(TSubclassOf<UMenuWidget> MenuClass);

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void ExitMenu();

	void StartSession();

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Success);

	void StartHostSession(FName SessionName);

	void OnDestroySessionCompleted(FName SessionName, bool Success);

	void OnFindSessionsCompleted(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();


private:
	TSubclassOf<UUserWidget> MainMenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;

	UMenuWidget* CurrentMenu;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	bool bIsHosting = false;

	FString DesiredServerName;

};
