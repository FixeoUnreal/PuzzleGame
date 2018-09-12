// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include <TextBlock.h>



void UServerRow::SetServerNameText(const FString& ServerName)
{
	ServerNameTextBlock->SetText(FText::FromString(ServerName));
}
