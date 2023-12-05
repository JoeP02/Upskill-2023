// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "EOSGameInstance.h"
#include "JoinGameScreen.h"
#include "ServerRow.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include <Kismet/GameplayStatics.h>

UMainMenu::UMainMenu()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UI/WBP_SessionResult"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ensure(btn_Login != nullptr)) return;
	btn_Login->OnClicked.AddDynamic(this, &UMainMenu::Login);

	if (!ensure(btn_HostGame != nullptr)) return;
	btn_HostGame->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(btn_JoinGame != nullptr)) return;
	btn_JoinGame->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(btn_Quit != nullptr)) return;
	btn_Quit->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);
	
	JoinGameButton = WBP_JoinGameScreen->btn_JoinSelectedGame;
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	HostGameButton = WBP_HostGameScreen->btn_HostGame;
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
}

void UMainMenu::Login()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Login();
	}
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		// FString* Map = Maps.Find("null");
		// FString* GameMode = GameModes.Find("null");

		FString ServerName = "Session Name Test";
		MenuInterface->Host(ServerName, FString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: Menu Interface is nullptr"));
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	// WBP_JoinGameScreen->SessionList->ClearChildren();

	UE_LOG(LogTemp, Warning, TEXT("Found Sessions: %d"), ServerNames.Num());

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (Row == nullptr)
		{
			Cast<UEOSGameInstance>(GetGameInstance())->CreateErrorScreen("An Error Occured - Please Try Again.");
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Session Data Received - Name: %s, Host Username: %s, Ping: %i"), *ServerData.Name, *ServerData.HostUsername, ServerData.Ping);

		Row->txt_ServerName->SetText(FText::FromString(ServerData.Name));
		Row->txt_ServerHost->SetText(FText::FromString(ServerData.HostUsername));
		Row->txt_PlayerCount->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers)));
		Row->txt_Ping->SetText(FText::FromString(FString::Printf(TEXT("%dms"), ServerData.Ping)));
		Row->Setup(this, i);
		++i;

		WBP_JoinGameScreen->SessionList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("New Selection"));
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < WBP_JoinGameScreen->SessionList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UServerRow>(WBP_JoinGameScreen->SessionList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	} 
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index: %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index Not Set"));
	}
}

void UMainMenu::OpenJoinMenu()
{
	
	if (!ensure(ContextMenu != nullptr)) return;
	if (!ensure(WBP_JoinGameScreen != nullptr)) return;
	ContextMenu->SetActiveWidget(WBP_JoinGameScreen);
	if (MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Open Join Menu - Ready To Refresh"));
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(ContextMenu != nullptr)) return;
	if (!ensure(WBP_HostGameScreen != nullptr)) return;
	ContextMenu->SetActiveWidget(WBP_HostGameScreen);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

