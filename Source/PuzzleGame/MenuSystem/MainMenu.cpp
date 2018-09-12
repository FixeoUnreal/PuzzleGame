// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "MenuInterface.h"
#include <WidgetSwitcher.h>
#include <Components/InputComponent.h>
#include <ScrollBox.h>
#include <ConstructorHelpers.h>
#include <TextBlock.h>
#include "MenuSystem/ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class)) { return; }
	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	TearDown();
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
	if (!ensure(BtnQuit)) { return false; }
	BtnQuit->OnClicked.AddDynamic(this, &UMainMenu::QuitClicked);

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
}

void UMainMenu::JoinClickedWithAddress()
{
	if (MenuInterface && ServerScrollBox)
	{
		if (!ensure(ServerRowClass)) { return; }
		UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(ServerRow)) { return; }
		ServerRow->SetServerNameText("Rowwwwwwwwww");
		ServerScrollBox->AddChild(ServerRow);
	}
}

void UMainMenu::BackToMainMenu()
{
	if (!ensure(MenuWidgetSwitcher)) { return; }
	if (!ensure(MainMenu)) { return; }
	MenuWidgetSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitClicked()
{
	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC)) { return; }

	PC->ConsoleCommand("Quit");
}
