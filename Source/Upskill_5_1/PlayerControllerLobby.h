// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobyWidget.h"
#include "PlayerInfo.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerLobby.generated.h"

/**
 * 
 */
UCLASS()
class UPSKILL_5_1_API APlayerControllerLobby : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION() virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION(Client, Reliable) void UpdatePlayerList(const TArray<FPlayerInfo> &InPlayerInfo);
	UFUNCTION(Client, Unreliable) void KickedFromLobby();

	UFUNCTION(Server, Unreliable) void SetReadyStatus();

	UPROPERTY(EditAnywhere) TArray<FPlayerInfo> ConnectedPlayerInfo;
	UPROPERTY(EditAnywhere) FPlayerInfo PlayerInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ULobyWidget* LobbyWidget;
};
