// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCardMenuClient.h"

void UPlayerCardMenuClient::NativeConstruct()
{
	Super::NativeConstruct();
	
	ViewProfileButton->OnHovered.AddDynamic(this, &UPlayerCardMenuClient::ClearTimer);
	ViewProfileButton->OnUnhovered.AddDynamic(this, &UPlayerCardMenuClient::SetTimer);
}

void UPlayerCardMenuClient::SetTimer()
{
	Super::SetTimer();
}

void UPlayerCardMenuClient::ClearTimer()
{
	Super::ClearTimer();
}
