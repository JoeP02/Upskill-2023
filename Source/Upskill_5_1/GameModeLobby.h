// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllerLobby.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeLobby.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API AGameModeLobby : public AGameModeBase
{
	GENERATED_BODY()

public:
	TArray<APlayerControllerLobby*> ConnectedPlayers;
	TArray<FPlayerInfo> ConnectedPlayersInfo;

	UFUNCTION() void UpdatePlayerLists();
	UFUNCTION() void UpdatePlayerListsTimerElapsed();
	
	UFUNCTION() void OnPostLogin(AController* NewPlayer) override;
	UFUNCTION() void Logout(AController* Exiting) override;
};
