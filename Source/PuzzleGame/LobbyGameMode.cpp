// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include <Engine/World.h>
#include "TimerManager.h"
#include <OnlineSessionInterface.h>
#include "PuzzlePlatformsGameInstance.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerCount++;
	if (PlayerCount >= 2)
	{
		FTimerHandle TimerHandle_StartSession;
		GetWorldTimerManager().SetTimer(TimerHandle_StartSession, this, &ALobbyGameMode::StartGame, 10.f);

	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayerCount--;
}

void ALobbyGameMode::StartGame()
{
	// Stop players from joining
	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if(!ensure(GameInstance)){ return; }
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Game?listen");
}
