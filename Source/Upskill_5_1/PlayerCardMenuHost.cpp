// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCardMenuHost.h"

#include "Components/Button.h"

void UPlayerCardMenuHost::NativeConstruct()
{
	Super::NativeConstruct();

	KickButton->OnHovered.AddDynamic(this, &UPlayerCardMenuHost::ClearTimer);
	KickButton->OnUnhovered.AddDynamic(this, &UPlayerCardMenuHost::SetTimer);

	ViewProfileButton->OnHovered.AddDynamic(this, &UPlayerCardMenuHost::ClearTimer);
	ViewProfileButton->OnUnhovered.AddDynamic(this, &UPlayerCardMenuHost::SetTimer);
}

void UPlayerCardMenuHost::SetTimer()
{
	Super::SetTimer();
}

void UPlayerCardMenuHost::ClearTimer()
{
	Super::ClearTimer();
}
