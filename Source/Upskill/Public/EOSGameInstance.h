// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuInterface.h"
#include "ErrorMenu.h"
#include "EOSGameInstance.generated.h"

UENUM()
enum class ELoginType : uint8
{
	VE_Developer UMETA(DisplayName="Developer"),
	VE_AccountPortal UMETA(DisplayName="Account Portal"),
};

/**
 * 
 */
UCLASS()
class UPSKILL_API UEOSGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	/*
	 **** Setup and Shutdown ****
	 */
	UEOSGameInstance(const FObjectInitializer &ObjectInitializer);
	virtual void Init() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable) AActor* GetDefaultActorObject(TSubclassOf<AActor> Actor);

	virtual void Shutdown() override;

	UPROPERTY(EditAnywhere) ELoginType LoginType;

	/*
	 **** Loading Screen ****
	 */
	UFUNCTION() virtual void BeginLoadingScreen(const FString& LevelName);
	UFUNCTION() virtual void EndLoadingScreen(UWorld* InLevelLoaded);

	UPROPERTY(BlueprintReadWrite, EditAnywhere) FSlateBrush BackgroundImage;

	/*
	 **** Error Screen ****
	 */
	void CreateErrorScreen(FString ErrorMessage);

	/*
	 **** Online ****
	 */
	UFUNCTION(Exec) void Host(FString ServerName, FString ServerAddress) override;
	UFUNCTION(Exec) void Join(uint32 Index) override;
	UFUNCTION(Exec) void RefreshServerList() override;
	UFUNCTION(Exec) virtual void LoadMainMenu() override;

	UFUNCTION(BlueprintCallable) void LoadMenuWidget();
	void StartSession();
	
	UFUNCTION(BlueprintCallable) void Login() override;
	UFUNCTION(BlueprintCallable) void CreateSession();
	UFUNCTION(BlueprintCallable) void DestroySession();
	UFUNCTION(BlueprintCallable) void FindSession();

	UFUNCTION(BlueprintCallable) void GetAllFriends();
	UFUNCTION(BlueprintCallable) void ShowInviteUI();
	UFUNCTION(BlueprintCallable) void ShowFriendUI();
	

	UPROPERTY(EditAnywhere) TSubclassOf<UErrorMenu> ErrorScreen;
	UErrorMenu* ErrorScreenInstance;
	
	UPROPERTY(Replicated) int32 numberOfPlayers;
	TSharedPtr<FOnlineSessionSearch> SearchSettings;
	
	UPROPERTY(EditAnywhere) bool bIsLAN = false;

	FString DesiredServerName;
	FString DesiredServerAddress;

protected:
	IOnlineSubsystem* OnlineSubsystem;
	IOnlineSessionPtr SessionInterface;

	bool bIsLoggedIn;

private:
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& Error);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	
	TSubclassOf<class UUserWidget> MainMenu;
	class UMainMenu* MainMenuWidget;
};