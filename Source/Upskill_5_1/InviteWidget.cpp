// Fill out your copyright notice in the Description page of Project Settings.


#include "InviteWidget.h"

#include "EOSGameInstance.h"
#include "PlayerInviteSlot.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"


UInviteWidget::UInviteWidget()
{
	ConstructorHelpers::FClassFinder<UUserWidget> InviteRowBPClass(TEXT("/Game/UI/WBP_InvitePlayerSlot"));
	if (!ensure(InviteRowBPClass.Class != nullptr)) return;

	InviteSlotClass = InviteRowBPClass.Class;
}

bool UInviteWidget::Initialize()
{
	return Super::Initialize();
}

void UInviteWidget::RefreshFriendList()
{
	for (TSharedRef<FOnlineFriend> Friend : PlayerFriends)
	{
		UPlayerInviteSlot* Row = CreateWidget<UPlayerInviteSlot>(GetWorld(), InviteSlotClass);

		Row->FriendData = Friend;

		if (Friend->GetPresence().bIsOnline)
		{
			OnlineFriendList->AddChild(Row);
		}
		else
		{
			OfflineFriendList->AddChild(Row);
		}

		

		UE_LOG(LogTemp, Warning, TEXT("Adding Friend With Name: %s"), *Friend->GetDisplayName());
	}
}

void UInviteWidget::GetAllFriends()
{
	IOnlineSubsystem* OnlineSubsystem = Cast<UEOSGameInstance>(GetGameInstance())->OnlineSubsystem;
	
	if (OnlineSubsystem)
	{
		if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
		{
			FriendsPtr->ReadFriendsList(0, ToString(EFriendsLists::Default), FOnReadFriendsListComplete::CreateUObject(this, &UInviteWidget::OnReadFriendsListComplete));
		}
	}
}

void UInviteWidget::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName,
												 const FString& Error)
{
	IOnlineSubsystem* OnlineSubsystem = Cast<UEOSGameInstance>(GetGameInstance())->OnlineSubsystem;
	
	if (bWasSuccessful)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
			{
				TArray<TSharedRef<FOnlineFriend>> FriendsList;
				if (FriendsPtr->GetFriendsList(0, ListName, FriendsList))
				{
					PlayerFriends = FriendsList;

					for (auto OnlineFriend : FriendsList)
					{
						UE_LOG(LogTemp, Warning, TEXT("Friend: %s"), *OnlineFriend->GetDisplayName());
					}
					
					RefreshFriendList();
				}
			}
		}
	}
}

