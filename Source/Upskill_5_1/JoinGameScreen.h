// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "JoinGameScreen.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API UJoinGameScreen : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UPROPERTY(Meta = (BindWidget))
	class UPanelWidget* SessionList;

	UPROPERTY(Meta = (BindWidget))
	class UButton* btn_JoinSelectedGame;
};
