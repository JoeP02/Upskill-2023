// Fill out your copyright notice in the Description page of Project Settings.


#include "LobyWidget.h"

#include "PlayerCard.h"
#include "Components/PanelWidget.h"

ULobyWidget::ULobyWidget()
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerSlotBPClass(TEXT("/Game/UI/WBP_PlayerCard"));
	if (!ensure(PlayerSlotBPClass.Class != nullptr)) return;

	PlayerSlotClass = PlayerSlotBPClass.Class;
}

void ULobyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void ULobyWidget::UpdatePlayerList(TArray<FPlayerInfo> PlayerInfo)
{
	PlayerList->ClearChildren();

	for (FPlayerInfo Player : PlayerInfo)
	{
		UPlayerCard* PlayerCard = CreateWidget<UPlayerCard>(GetWorld(), PlayerSlotClass);
		PlayerCard->PlayerInfo = Player;

		PlayerList->AddChild(PlayerCard);
	}
}
