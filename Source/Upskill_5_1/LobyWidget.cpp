// Fill out your copyright notice in the Description page of Project Settings.


#include "LobyWidget.h"

#include "PlayerCard.h"
#include "PlayerCardMenuClient.h"
#include "PlayerCardMenuHost.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"

ULobyWidget::ULobyWidget()
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerSlotBPClass(TEXT("/Game/UI/WBP_PlayerCard"));
	if (!ensure(PlayerSlotBPClass.Class != nullptr)) return;

	PlayerSlotClass = PlayerSlotBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> PlayerCardMenuHostBPClass(TEXT("/Game/UI/WBP_PlayerCardMenuHost"));
	if (!ensure(PlayerCardMenuHostBPClass.Class != nullptr)) return;

	PlayerCardMenuHostClass = PlayerCardMenuHostBPClass.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerCardMenuClientBPClass(TEXT("/Game/UI/WBP_PlayerCardMenuClient"));
	if (!ensure(PlayerCardMenuClientBPClass.Class != nullptr)) return;

	PlayerCardMenuClientClass = PlayerCardMenuClientBPClass.Class;
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

		PlayerCard->OnPlayerCardSelected.AddDynamic(this, &ULobyWidget::OnPlayerCardClicked);
	}
}

void ULobyWidget::OnPlayerCardClicked(FPlayerInfo PlayerInfo)
{
	HidePlayerCardMenu();
	
	if (GetWorld()->GetFirstPlayerController()->HasAuthority())
	{
		PlayerCardMenu = CreateWidget<UPlayerCardMenu>(GetWorld(), PlayerCardMenuHostClass);

		UE_LOG(LogTemp, Error, TEXT("UPSKILL - Host"))
	}
	else
	{
		PlayerCardMenu = CreateWidget<UPlayerCardMenu>(GetWorld(), PlayerCardMenuClientClass);

		UE_LOG(LogTemp, Error, TEXT("UPSKILL - Client"))
	}
	
	PlayerCardMenu->PlayerInfo = PlayerInfo;

	UCanvasPanelSlot* PanelSlot = MenuCanvas->AddChildToCanvas(PlayerCardMenu);
	PanelSlot->SetAutoSize(true);
	PanelSlot->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld())); 
}

void ULobyWidget::HidePlayerCardMenu()
{
	if (PlayerCardMenu)
	{
		PlayerCardMenu->RemoveFromParent();
	}
}
