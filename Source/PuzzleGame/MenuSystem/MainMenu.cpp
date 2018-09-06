// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"



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
}

void UMainMenu::JoinClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join clicked"));
}
