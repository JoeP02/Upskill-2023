// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "PlayerCardMenu.h"
#include "PlayerInfo.h"
#include "Components/CanvasPanel.h"
#include "LobyWidget.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API ULobyWidget : public UMenuWidget
{
	GENERATED_BODY()

public:

	ULobyWidget();
	
	virtual void NativeConstruct() override;
	virtual void UpdatePlayerList(TArray<FPlayerInfo> PlayerInfo);
	
	void HidePlayerCardMenu();

	UFUNCTION() void OnPlayerCardClicked(FPlayerInfo PlayerInfo);

	TSubclassOf<UUserWidget> PlayerSlotClass;
	TSubclassOf<UUserWidget> PlayerCardMenuHostClass;
	TSubclassOf<UUserWidget> PlayerCardMenuClientClass;

	UPROPERTY(Meta = (BindWidget)) UCanvasPanel* MenuCanvas;
	UPROPERTY(Meta = (BindWidget)) UPanelWidget* PlayerList;
	UPROPERTY(EditAnywhere) UPlayerCardMenu* PlayerCardMenu;
};

