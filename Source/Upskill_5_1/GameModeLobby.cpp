// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeLobby.h"

void AGameModeLobby::UpdatePlayerLists()
{
	ConnectedPlayersInfo.Empty();

	for (APlayerControllerLobby* Player : ConnectedPlayers)
	{
		if (Player->IsLocalController())
		{
			FPlayerInfo TempInfo;
			TempInfo.PlayerName = FText::FromString("Host");
			TempInfo.bIsReady = true;

			ConnectedPlayersInfo.Add(TempInfo);
		}
		else
		{
			ConnectedPlayersInfo.Add(Player->PlayerInfo);
		}
	}

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGameModeLobby::UpdatePlayerListsTimerElapsed, 0.2, false);
}

void AGameModeLobby::OnPostLogin(AController* NewPlayer)
{
	APlayerControllerLobby* NewPlayerController = Cast<APlayerControllerLobby>(NewPlayer);

	ConnectedPlayers.Add(NewPlayerController);

	if (NewPlayerController->IsLocalController())
	{
		FPlayerInfo TempInfo;
		TempInfo.PlayerName = FText::FromString("Host");
		TempInfo.bIsReady = true;

		NewPlayerController->PlayerInfo = TempInfo;

		ConnectedPlayersInfo.Add(TempInfo);
	}
	else
	{
		ConnectedPlayersInfo.Add(NewPlayerController->PlayerInfo);
	}

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGameModeLobby::UpdatePlayerListsTimerElapsed, 0.2, false);
}

void AGameModeLobby::UpdatePlayerListsTimerElapsed()
{
	for (APlayerControllerLobby* Player : ConnectedPlayers)
	{
		Player->UpdatePlayerList(ConnectedPlayersInfo);
	}
}

void AGameModeLobby::Logout(AController* Exiting)
{
	APlayerControllerLobby* ExitingController = Cast<APlayerControllerLobby>(Exiting);

	ConnectedPlayers.Remove(ExitingController);

	UpdatePlayerLists();
}
