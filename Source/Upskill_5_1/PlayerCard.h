// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCard.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API UPlayerCard : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FPlayerInfo PlayerInfo;
};
