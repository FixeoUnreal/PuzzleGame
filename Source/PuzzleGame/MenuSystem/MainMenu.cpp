// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "MenuInterface.h"



void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface)
{
	UE_LOG(LogTemp, Warning, TEXT("Set interface called"));
	this->MenuInterface = MenuInterface;
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if(!ensure(World)){ return; }
	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC)) { return; }

	FInputModeUIOnly InputModeUI;
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeUI.SetWidgetToFocus(this->TakeWidget());

	PC->SetInputMode(InputModeUI);
	PC->bShowMouseCursor = true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	FInputModeGameOnly InputModeGameOnly;

	if(!ensure(InWorld)){ return; }
	APlayerController* PC = InWorld->GetFirstPlayerController();
	if (!ensure(PC)) { return; }

	PC->SetInputMode(InputModeGameOnly);
	PC->bShowMouseCursor = false;

	this->RemoveFromViewport();
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success){ return false; }

	if(!ensure(BtnHost)){ return false; }
	BtnHost->OnClicked.AddDynamic(this, &UMainMenu::HostClicked);
	if (!ensure(BtnJoin)) { return false; }
	BtnJoin->OnClicked.AddDynamic(this, &UMainMenu::JoinClicked);

	UE_LOG(LogTemp, Warning, TEXT("Initilize called"));
	return true;
}

void UMainMenu::HostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host clicked"));
	if (MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("yeah interface"));
		MenuInterface->Host();
	}
}

void UMainMenu::JoinClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join clicked"));
	/*if (MenuInterface)
	{
		MenuInterface->Join();
	}*/
}
