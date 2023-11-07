// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorMenu.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_API UErrorMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FText ErrorMessageToDisplay;
};
