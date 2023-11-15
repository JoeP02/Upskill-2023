// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"

#include "DeveloperLoginCredentials.h"
#include "MainMenu.h"
#include "OnlineSubsystem.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

const FName DemoSessionName = FName("Test Session");
const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UEOSGameInstance::UEOSGameInstance(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	
	MainMenu = MenuBPClass.Class;
	
	bIsLoggedIn = false;

	OnSessionInviteReceived = FOnSessionInviteReceivedDelegate::CreateUObject(this, &UEOSGameInstance::SessionInviteReceived);
	OnSessionUserInviteAccepted = FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UEOSGameInstance::SessionUserInviteAccepted);
}


void UEOSGameInstance::Init()
{
	Super::Init();
/*
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UEOSGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UEOSGameInstance::EndLoadingScreen);
*/

	OnlineSubsystem = IOnlineSubsystem::Get();
	SessionInterface = OnlineSubsystem->GetSessionInterface();
	OnlineUserInterface = OnlineSubsystem->GetUserInterface();
	
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnJoinSessionComplete);

		OnSessionInviteReceivedHandle = SessionInterface->AddOnSessionInviteReceivedDelegate_Handle(OnSessionInviteReceived);
		OnSessionUserInviteAcceptedHandle = SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(OnSessionUserInviteAccepted);
	}

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UEOSGameInstance::OnNetworkFailure);
	}
	
	Login();
}

void UEOSGameInstance::LoadMenuWidget()
{
	if (!ensure(MainMenu != nullptr)) return;
	
	MainMenuWidget = CreateWidget<UMainMenu>(this, MainMenu);
	if (!ensure(MainMenuWidget != nullptr)) return;

	MainMenuWidget->Setup();	

	MainMenuWidget->SetMenuInterface(this);
}

void UEOSGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEOSGameInstance, numberOfPlayers);
}

void UEOSGameInstance::BeginLoadingScreen(const FString& LevelName)
{
	/*
	// UE_LOG(LogTemp, Warning, TEXT("Loading Level: %s"), *LevelName);
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	// LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	LoadingScreen.WidgetLoadingScreen = SNew(SLoadingScreenWidget);

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	*/
}

void UEOSGameInstance::EndLoadingScreen(UWorld* InLoadedLevel)
{

}

void UEOSGameInstance::Host(FString ServerName, FString ServerAddress)
{
	UE_LOG(LogTemp, Warning, TEXT("Host Called Game Instance"));
	
	DesiredServerName = ServerName;
	
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
		else
		{
			CreateSession();
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("JPG - Failed at Host()"));
}

void UEOSGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	if (!SessionSearch.IsValid())
	{
		return;
	}
	
	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->Teardown();
	}
	
	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
}

void UEOSGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(NAME_GameSession);
	}
}

void UEOSGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		// SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FString("UpskillLobby"), EOnlineComparisonOp::Equals);
		SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Session Find"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UEOSGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

FText UEOSGameInstance::GetPlayerUsername()
{
	return PlayerUsername;
}

AActor* UEOSGameInstance::GetDefaultActorObject(TSubclassOf<AActor> Actor)
{
	return Actor.GetDefaultObject();
}

void UEOSGameInstance::Shutdown()
{
	DestroySession();
	
	Super::Shutdown();
}

void UEOSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			if (Identity->GetLoginStatus(0) == ELoginStatus::Type::LoggedIn)
			{
				Identity->Logout(0);
			}
			
			FOnlineAccountCredentials Credentials;
			
			
			if (LoginType == ELoginType::VE_Developer)
			{
				const UDeveloperLoginCredentials* LoginCredentials = GetDefault<UDeveloperLoginCredentials>();
				Credentials.Type = FString("developer");
				Credentials.Id = LoginCredentials->IPAddress + ":" + LoginCredentials->Port;
				Credentials.Token = LoginCredentials->Token;
			}
			else if (LoginType == ELoginType::VE_AccountPortal)
			{
				Credentials.Type = FString("accountportal");
				Credentials.Id = FString();
				Credentials.Token = FString();
			}
			
			Identity->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
	const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Logged In: %d"), bWasSuccessful);
	bIsLoggedIn = bWasSuccessful;
	
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			PlayerUsername = FText::FromString(Identity->GetPlayerNickname(0));
			
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}
	}
}

void UEOSGameInstance::CreateSession()
{
	if (bIsLoggedIn)
	{
		if (SessionInterface.IsValid())
		{
			FOnlineSessionSettings SessionSettings;

			if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
			{
				SessionSettings.bIsLANMatch = true;
			}
			else
			{
				SessionSettings.bIsLANMatch = false;
			}

			if (bIsLAN)
			{
				SessionSettings.bIsLANMatch = true;
			}
		
			SessionSettings.NumPublicConnections = 4;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bUseLobbiesIfAvailable = true;
			SessionSettings.bIsDedicated = false;
			SessionSettings.bAllowInvites = true;
			SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			SessionSettings.Set(SEARCH_KEYWORDS, FString("UpskillLobby"), EOnlineDataAdvertisementType::ViaOnlineService);
		
			SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);	
		}
	}
	else
	{
		CreateErrorScreen("User Not Logged In. Please Login and Try Again.");
	}
}

void UEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		CreateErrorScreen("An Unknown Error Occured. Please Check Your Network Connection and Try Again.");
		return;
	}
	
	if (MainMenuWidget != nullptr)
	{
		// MainMenuWidget->Teardown();
	}
	
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Levels/Lobby?listen");

	UE_LOG(LogTemp, Warning, TEXT("JPG - End Of OnCreateSessionComplete"));
}

void UEOSGameInstance::DestroySession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnDestroySessionComplete);
				SessionPtr->DestroySession(DemoSessionName);
			}
		}
	}
}

void UEOSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		CreateSession();
	}
}

void UEOSGameInstance::FindSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("UpskillLobby"), EOnlineComparisonOp::Equals);
				SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
				SearchSettings->bIsLanQuery = bIsLAN;
				
				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindSessionsComplete);
				SessionPtr->FindSessions(0, SearchSettings.ToSharedRef());

				//SessionPtr->SendSessionInviteToFriend()
			}
		}
	}
}

void UEOSGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (SessionSearch.IsValid())
		{
			if (MainMenuWidget != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Finished Session Find"));

				TArray<FServerData> ServerNames;
				for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Session: %s"), *SearchResult.GetSessionIdStr());
					FServerData Data;
					Data.Name = *SearchResult.GetSessionIdStr();
					Data.HostUsername = SearchResult.Session.OwningUserName;
					Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
					Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
					Data.Ping = SearchResult.PingInMs;
					FString ServerName;
					if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
					{
						Data.Name = ServerName;
					}
					else
					{
						Data.Name = "Name Unavailable";
					}

					UE_LOG(LogTemp, Warning, TEXT("Session Data Sent - Name: %s, Host Username: %s, Ping: %i"), *Data.Name, *Data.HostUsername, Data.Ping);
			
					ServerNames.Add(Data);
				}

				MainMenuWidget->SetServerList(ServerNames);
			}
			else
			{
				CreateErrorScreen("MainMenuWidget Is nullptr - Please Try Again");
			}
		}
		else
		{
			CreateErrorScreen("Session Search Was Not Valid - Please Try Again.");
		}
	}
	else
	{
		CreateErrorScreen("Find Session Was Not Successful - Please Try Again.");
	}
}

void UEOSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	
	FString Address;
	if(!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could Not Get Connect String"));
		return;
	}
	
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UEOSGameInstance::GetAllFriends()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
			{
				FriendsPtr->ReadFriendsList(0, ToString(EFriendsLists::Default), FOnReadFriendsListComplete::CreateUObject(this, &UEOSGameInstance::OnReadFriendsListComplete));
			}
		}
	}
}

void UEOSGameInstance::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName,
                                                 const FString& Error)
{
	if (bWasSuccessful)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
			{
				TArray<TSharedRef<FOnlineFriend>> FriendsList;
				if (FriendsPtr->GetFriendsList(0, ListName, FriendsList))
				{
					AllFriends = FriendsList;
				}
			}
		}
	}
}

void UEOSGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	LoadMainMenu();
}

void UEOSGameInstance::SessionInviteReceived(const FUniqueNetId& UserId, const FUniqueNetId& FriendId, const FString& String,
	const FOnlineSessionSearchResult& SearchResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Invite Recieved"));
}

void UEOSGameInstance::SessionUserInviteAccepted(bool bWasSuccessful, int UserNum, TSharedPtr<const FUniqueNetId> FriendId, const FOnlineSessionSearchResult& SearchResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Invite Accepted"));

	SessionInterface->JoinSession(0, NAME_GameSession, SearchResult);
}

void UEOSGameInstance::CreateErrorScreen(FString ErrorMessage)
{
	ErrorScreenInstance = CreateWidget<UErrorMenu>(GetWorld(), ErrorScreen);
	ErrorScreenInstance->ErrorMessageToDisplay = FText::FromString(ErrorMessage);
	ErrorScreenInstance->AddToViewport();
}

void UEOSGameInstance::ShowInviteUI()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
			{
				UIPtr->ShowInviteUI(0, DemoSessionName);
			}
		}
	}
}

void UEOSGameInstance::ShowFriendUI()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
			{
				UIPtr->ShowFriendsUI(0);
			}
		}
	}
}