// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerInviteSlot.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_API UPlayerInviteSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	
	UPROPERTY(meta=(BindWidget)) UButton* InviteButton;
	
	TSharedPtr<FOnlineFriend> FriendData;

	UFUNCTION(BlueprintCallable) FText GetPlayerName();
	UFUNCTION(BlueprintCallable) void OnInviteButtonPressed();
};
