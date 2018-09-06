// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init() override;

	UFUNCTION(Exec)
	virtual void Host() override;

	UFUNCTION(Exec)
	virtual void Join(const FString Address) override;

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void LoadMenu();

private:
	TSubclassOf<UUserWidget> MenuClass;

};
