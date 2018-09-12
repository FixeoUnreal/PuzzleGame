// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

class UTextBlock;
class UButton;
class UMainMenu;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ServerNameTextBlock;
	
	void Setup(UMainMenu* Parent, uint32 Index);

protected:
	uint32 RowIndex = -1;

	UMainMenu* MainMenu;

	UPROPERTY(meta=(BindWidget))
	UButton* BtnChooseServer;

protected:
	UFUNCTION()
	void ChooseServerClicked();
};
