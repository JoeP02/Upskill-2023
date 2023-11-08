// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInviteSlot.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Invite %s To Game"), *GetPlayerName().ToString());
}
