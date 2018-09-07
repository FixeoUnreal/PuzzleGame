// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "MenuInterface.h"
#include <WidgetSwitcher.h>
#include <EditableTextBox.h>
#include <Components/InputComponent.h>

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface)
{
	UE_LOG(LogTemp, Warning, TEXT("Set interface called"));
	this->MenuInterface = MenuInterface;
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	// Set input mode
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

	// Bind onclick events
	if(!ensure(BtnHost)){ return false; }
	BtnHost->OnClicked.AddDynamic(this, &UMainMenu::HostClicked);
	if (!ensure(BtnJoin)) { return false; }
	BtnJoin->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	if (!ensure(BtnJoinWithAddr)) { return false; }
	BtnJoinWithAddr->OnClicked.AddDynamic(this, &UMainMenu::JoinClickedWithAddress);
	if(!ensure(BtnBack)){ return false; }
	BtnBack->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

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

void UMainMenu::OpenJoinMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Join Menu"));
	if (!ensure(MenuWidgetSwitcher)) { return; }
	if(!ensure(JoinMenu)){ return; }
	MenuWidgetSwitcher->SetActiveWidget(JoinMenu);

	if (!ensure(IPAddressTextBox)) { return; }
	IPAddressTextBox->SetKeyboardFocus();
}

void UMainMenu::JoinClickedWithAddress()
{
	if(!ensure(IPAddressTextBox)){ return; }
	const FString& IPAdress = IPAddressTextBox->GetText().ToString();

	if (MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("yeah interface"));
		MenuInterface->Join(IPAdress);
	}
}

void UMainMenu::BackToMainMenu()
{
	if (!ensure(MenuWidgetSwitcher)) { return; }
	if (!ensure(MainMenu)) { return; }
	MenuWidgetSwitcher->SetActiveWidget(MainMenu);
}
