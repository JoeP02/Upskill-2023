// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCardMenu.h"

void UPlayerCardMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetTimer();
}

void UPlayerCardMenu::SetTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UPlayerCardMenu::MenuTimeout, 3.0f, false);
}

void UPlayerCardMenu::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UPlayerCardMenu::MenuTimeout()
{
	RemoveFromParent();
}
