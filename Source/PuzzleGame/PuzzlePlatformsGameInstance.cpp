// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include <Engine/Engine.h>
#include <OnlineSessionSettings.h>
#include <GameFramework/PlayerController.h>
#include <ConstructorHelpers.h>
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuInGame.h"
#include "MenuSystem/MenuWidget.h"


const static FName SESSION_NAME = TEXT("My Session Game");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class)) { return; }
	MainMenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_MenuInGame"));
	if (!ensure(InGameMenuBPClass.Class)) { return; }
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init..."));
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionCompleted);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionCompleted);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsCompleted);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found!"));
	}

}

void UPuzzlePlatformsGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}


void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if(!SessionInterface.IsValid() || !SessionSearch.IsValid()){ return; }
	if (CurrentMenu)
	{
		CurrentMenu->TearDown();
	}

	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterface->RemoveNamedSession(SESSION_NAME);
		}
	}
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

}

void UPuzzlePlatformsGameInstance::OpenMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }
	PC->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	if(!SessionInterface.IsValid()){ return; }
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Start find sessions"));
	}
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget(TSubclassOf<UMenuWidget> MenuClass)
{
	if (!ensure(MenuClass)) { return; }
	CurrentMenu = CreateWidget<UMenuWidget>(this, MenuClass);
	if (!ensure(CurrentMenu)) { return; }

	CurrentMenu->Setup();

	CurrentMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::ExitMenu()
{
	if (!ensure(CurrentMenu)) { return; }
	CurrentMenu->TearDown();
}

void UPuzzlePlatformsGameInstance::OnCreateSessionCompleted(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("On creation completed"));
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creation succeeded"));
		StartHostSession(SessionName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not create session"));
	}
}

void UPuzzlePlatformsGameInstance::StartHostSession(FName SessionName)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter Start Session"));
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, TEXT("Hosting " + SessionName.ToString()));

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

}

void UPuzzlePlatformsGameInstance::OnDestroySessionCompleted(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySession success"));
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsCompleted(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Find sessions completed"));
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find sessions successful"));
		TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
		
		if(!ensure(CurrentMenu)){ return; }
		UMainMenu* MainMenu = Cast<UMainMenu>(CurrentMenu);
		if(!ensure(MainMenu)){ return; }
		MainMenu->SetServerList(Results);
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success 
	&& SessionInterface.IsValid())
	{
		FString ConnectionInfo;
		bool Succecss = SessionInterface->GetResolvedConnectString(SessionName, ConnectionInfo);
		if(!Succecss || ConnectionInfo.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get connect string!"));
			return; 
		}

		// Join match
		UEngine* Engine = GetEngine();
		if (!ensure(Engine)) { return; }

		Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *ConnectionInfo));

		APlayerController* PC = GetFirstLocalPlayerController();
		if (!ensure(PC)) { return; }

		PC->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);

	}
}

