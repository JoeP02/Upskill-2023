// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "LobyWidget.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API ULobyWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
};

