// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCardMenu.h"
#include "Components/Button.h"
#include "PlayerCardMenuHost.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API UPlayerCardMenuHost : public UPlayerCardMenu
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void SetTimer() override;
	virtual void ClearTimer() override;
	
	UPROPERTY(Meta = (BindWidget)) UButton* KickButton;
	UPROPERTY(Meta = (BindWidget)) UButton* ViewProfileButton;
};
