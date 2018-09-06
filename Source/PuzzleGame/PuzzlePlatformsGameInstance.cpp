// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include <Engine/Engine.h>
#include <GameFramework/PlayerController.h>
#include <ConstructorHelpers.h>
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"




UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Gameinstance constructed"));

	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if(!ensure(MainMenuBPClass.Class)){ return; }

	MenuClass = MainMenuBPClass.Class;
	UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Gameinstance init"));
	UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if(!ensure(Engine)){ return; }

	Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if(!ensure(World)){ return; }
	
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	ExitMenu();
}

void UPuzzlePlatformsGameInstance::Join(const FString Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PC = GetFirstLocalPlayerController();
	if(!ensure(PC)){ return; }

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	ExitMenu();
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if(!ensure(MenuClass)){ return; }
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if(!ensure(Menu)){ return; }

	Menu->AddToViewport();

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }

	FInputModeUIOnly InputModeUI;
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeUI.SetWidgetToFocus(Menu->TakeWidget());

	PC->SetInputMode(InputModeUI);
	PC->bShowMouseCursor = true;

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::ExitMenu()
{
	FInputModeGameOnly InputModeGameOnly;

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }

	PC->SetInputMode(InputModeGameOnly);
	PC->bShowMouseCursor = false;
}
