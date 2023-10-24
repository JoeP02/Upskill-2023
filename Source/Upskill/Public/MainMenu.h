// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "MenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	uint16 Ping;
	FString HostUsername;
};

UCLASS()
class UPSKILL_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu();

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
		class UWidgetSwitcher* ContextMenu;

private:
	UPROPERTY(Meta = (BindWidget))
		class UButton* btn_Login;

	UPROPERTY(Meta = (BindWidget))
		class UButton* btn_HostGame;

	UPROPERTY(Meta = (BindWidget))
		class UButton* btn_JoinGame;

	UButton* JoinGameButton;

	/*
	UPROPERTY(Meta = (BindWidget))
	class UButton* HostGameButton;
	*/

	UPROPERTY(Meta = (BindWidget))
		class UButton* btn_Quit;

	// UJoinGameScreen* JoinGameScreen;

	/*
	UPROPERTY(Meta = (BindWidget))
		class UJoinGameScreen* WBP_JoinGameScreen;

		*/

	UPROPERTY(Meta = (BindWidget))
		class UWidget* WBP_HostGameScreen;

	/*
	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* SessionNameTextBox;
	*/

	/*
	UPROPERTY(Meta = (BindWidget))
	class UComboBoxString* HostMapCombo;

	UPROPERTY(Meta = (BindWidget))
	class UComboBoxString* HostGameModeCombo;
	*/

	TSubclassOf<UUserWidget> ServerRowClass;

	UFUNCTION()
		void Login();

	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenHostMenu();

	UFUNCTION()
		void QuitPressed();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();

	TMap<FString, FString> Maps;
	TMap<FString, FString> GameModes;
};