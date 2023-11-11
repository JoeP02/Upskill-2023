// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerLobby.h"

#include "GameModeLobby.h"

void APlayerControllerLobby::UpdatePlayerList_Implementation(const TArray<FPlayerInfo> &InPlayerInfo)
{
	ConnectedPlayerInfo = InPlayerInfo;

	LobbyWidget->UpdatePlayerList(ConnectedPlayerInfo);
}

void APlayerControllerLobby::OnPossess(APawn* InPawn)
{
	PlayerInfo.Character = InPawn;

	AGameModeLobby* GameModeLobby = Cast<AGameModeLobby>(GetWorld()->GetAuthGameMode());

	GameModeLobby->UpdatePlayerLists();
}

void APlayerControllerLobby::SetReadyStatus_Implementation()
{
	PlayerInfo.bIsReady = !(PlayerInfo.bIsReady);

	AGameModeLobby* GameModeLobby = Cast<AGameModeLobby>(GetWorld()->GetAuthGameMode());

	GameModeLobby->UpdatePlayerLists();
}

void APlayerControllerLobby::KickedFromLobby_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Kicked From Game"));
}
