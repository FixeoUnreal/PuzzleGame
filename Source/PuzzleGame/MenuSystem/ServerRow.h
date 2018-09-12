// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetServerNameText(const FString& ServerName);
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ServerNameTextBlock;
	
	
};
