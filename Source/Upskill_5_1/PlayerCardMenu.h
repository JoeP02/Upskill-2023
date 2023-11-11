// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "PlayerInfo.h"
#include "Components/VerticalBox.h"
#include "PlayerCardMenu.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API UPlayerCardMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UFUNCTION() virtual void NativeConstruct() override;
	UFUNCTION() virtual void SetTimer();
	UFUNCTION() virtual void ClearTimer();
	
	UFUNCTION() void MenuTimeout();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FPlayerInfo PlayerInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FTimerHandle TimerHandle;

	UPROPERTY(Meta = (BindWidget)) UVerticalBox* PlayerCardMenu;
};
