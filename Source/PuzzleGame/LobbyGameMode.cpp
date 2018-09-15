// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"




void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerCount++;
	if (PlayerCount == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("We have 3 players in da house"));
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayerCount--;
}
