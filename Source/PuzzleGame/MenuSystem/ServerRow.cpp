// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "MainMenu.h"
#include <Button.h>


void UServerRow::Setup(UMainMenu* Parent, uint32 Index)
{
	RowIndex = Index;
	MainMenu = Parent;
	BtnChooseServer->OnClicked.AddDynamic(this, &UServerRow::ChooseServerClicked);
}

void UServerRow::ChooseServerClicked()
{
	if(!ensure(MainMenu)){ return; }
	MainMenu->SelectIndex(RowIndex);
}
