// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCard.h"

void UPlayerCard::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCardButton->OnClicked.AddDynamic(this, &UPlayerCard::UPlayerCard::OnPlayerCardClicked);
}

void UPlayerCard::OnPlayerCardClicked()
{
	OnPlayerCardSelected.Broadcast(PlayerInfo);
}
