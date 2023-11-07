// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* txt_ServerName;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* txt_ServerHost;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* txt_PlayerCount;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* txt_Ping;

	UPROPERTY(BlueprintReadOnly)
		bool Selected = false;

	void Setup(class UMainMenu* Parent, uint32 indexIndex);
};
