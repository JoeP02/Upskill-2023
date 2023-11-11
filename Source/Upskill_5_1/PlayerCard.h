// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PlayerCard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCardSelected, FPlayerInfo, PlayerInfo);

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API UPlayerCard : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION() virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FPlayerInfo PlayerInfo;

	UPROPERTY(BlueprintCallable, Category="Event Dispatchers") FOnPlayerCardSelected OnPlayerCardSelected;

	UPROPERTY(meta=(BindWidget)) UButton* PlayerCardButton;
	UFUNCTION() void OnPlayerCardClicked();
};
