// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DeveloperLoginCredentials.generated.h"

/**
 * 
 */
UCLASS(Config=Engine, defaultconfig, meta = (DisplayName="Developer Account Login Settings"))
class UPSKILL_5_1_API UDeveloperLoginCredentials : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="General")
	FString IPAddress;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="General")
	FString Port;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="General")
	FString Token;

	UDeveloperLoginCredentials();
};
