// Fill out your copyright notice in the Description page of Project Settings.


#include "LobyWidget.h"

void ULobyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}
