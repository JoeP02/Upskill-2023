// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "OnlineSubsystemTypes.h"
#include "Blueprint/UserWidget.h"
#include "InviteWidget.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API UInviteWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	UInviteWidget();

	virtual bool Initialize() override;

	TArray<TSharedRef<FOnlineFriend>> PlayerFriends;

	TSubclassOf<UUserWidget> InviteSlotClass;

	UPROPERTY(Meta = (BindWidget)) class UPanelWidget* OnlineFriendList;
	UPROPERTY(Meta = (BindWidget)) class UPanelWidget* OfflineFriendList;

	UFUNCTION(BlueprintCallable) void RefreshFriendList();
	UFUNCTION(BlueprintCallable) void GetAllFriends();
	void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName,
	                               const FString& Error);
};
