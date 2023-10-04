// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EOSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UEOSGameInstance();

	virtual void Init() override;

	void Login();
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	UFUNCTION(BlueprintCallable)
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetAllFriends();
	void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& Error);

	UFUNCTION(BlueprintCallable)
	void ShowInviteUI();

	UFUNCTION(BlueprintCallable)
	void ShowFriendUI();

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	bool bIsLoggedIn;
	
};
