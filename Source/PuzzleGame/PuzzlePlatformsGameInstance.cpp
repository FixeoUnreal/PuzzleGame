// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include <Engine/Engine.h>
#include <OnlineSessionSettings.h>
#include <OnlineSessionInterface.h>
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
		
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
				SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsCompleted);
				UE_LOG(LogTemp, Warning, TEXT("Start find sessions"));
			}
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
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}


void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::OpenMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }
	PC->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
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
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsCompleted(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Find sessions completed"));
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find sessions successful"));
	}
}

