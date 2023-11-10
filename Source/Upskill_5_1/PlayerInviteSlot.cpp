// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInviteSlot.h"

UPlayerInviteSlot::UPlayerInviteSlot()
{
	
}

bool UPlayerInviteSlot::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return Success;
	}

	InviteButton->OnClicked.AddDynamic(this, &UPlayerInviteSlot::OnInviteButtonPressed);

	return true;
}

FText UPlayerInviteSlot::GetPlayerName()
{
	return FText::FromString(FriendData->GetDisplayName());
}

void UPlayerInviteSlot::OnInviteButtonPressed()
{
	SessionPtr = Cast<UEOSGameInstance>(GetGameInstance())->SessionInterface;

	if (SessionPtr == nullptr)
	{
		Cast<UEOSGameInstance>(GetGameInstance())->CreateErrorScreen("Could Not Get Session Interface - Please Try Again.");
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Invite %s To Game"), *GetPlayerName().ToString());

	SessionPtr->SendSessionInviteToFriend(0, NAME_GameSession, *FriendData->GetUserId());
}
