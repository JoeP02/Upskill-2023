// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EOSGameInstance.h"
#include "MenuWidget.h"
#include "OnlineSubsystemTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PlayerInviteSlot.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API UPlayerInviteSlot : public UMenuWidget
{
	GENERATED_BODY()

public:
	UPlayerInviteSlot();
	virtual bool Initialize() override;
	
	UPROPERTY(meta=(BindWidget)) UButton* InviteButton;
	
	TSharedPtr<FOnlineFriend> FriendData;
	IOnlineSessionPtr SessionPtr = nullptr;

	UFUNCTION(BlueprintCallable) FText GetPlayerName();
	UFUNCTION(BlueprintCallable) void OnInviteButtonPressed();
};
