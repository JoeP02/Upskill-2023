// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"

#include <Components/Button.h>

#include "HostGameScreen.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API UHostGameScreen : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize();

	UPROPERTY(Meta = (BindWidget)) UButton* btn_HostGameButton;
	
};
