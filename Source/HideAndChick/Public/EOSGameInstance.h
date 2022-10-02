// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "VoiceChat.h"
#include "EOSGameInstance.generated.h"


class FOnlineSessionSearch;


/**
 * 
 */
UCLASS()
class HIDEANDCHICK_API UEOSGameInstance : public UGameInstance
{ 
	GENERATED_BODY()
	

public:
	//constructor
	UEOSGameInstance();

protected:
	/*Online Subsystem*/
	class IOnlineSubsystem* OnlineSubsystem;

	/*Session Interface*/
	IOnlineSessionPtr SessionInterface;

	/*EOS Login Identity*/
	IOnlineIdentityPtr Identity;

	/*Friends Interface*/
	IOnlineFriendsPtr FriendsInterface;

	/*Find Session Settings*/
	TSharedPtr<FOnlineSessionSearch> SearchSettings;

	/*UI Interface*/
	IOnlineExternalUIPtr UIInterface;

	/*Voice Chat*/
	IVoiceChat* VoiceChat;

	/*Voice Chat User*/
	IVoiceChatUser* VoiceChatUser;
	

protected:
	/*Initialize*/
	virtual void Init() override;

	/*Bind On Login Complete (EOS)*/
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	/*Bind On Create Session Complete */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/*Bind On Destroy Session Complete */
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/*Bind On Get All Friends Complete */
	void OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	/*Bind On Find Session Complete*/
	void OnFindSessionComplete(bool bWasSuccessful);

	/*Bind On Join Session Complete*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/*Bind On Join Session Complete*/
	void OnVoiceLoginComplete(const FString&  PlayerName , const FVoiceChatResult&  Result);

	/*Bind On Join Session Complete*/
	void OnVoiceChatConnectComplete(const FVoiceChatResult& Result);

	

protected:
	/*Create Session*/
	UFUNCTION(BlueprintCallable)
	void CreateSession();

	/*Find Session*/
	UFUNCTION(BlueprintCallable)
	void FindSession();

	/*Destroy Session*/
	UFUNCTION(BlueprintCallable)
	void DestorySession();

	/*Get Friends List*/
	UFUNCTION(BlueprintCallable)
	void GetAllFriends();

	/*Show Friends UI)*/
	UFUNCTION(BlueprintCallable)
	void ShowUI();

	/*Show Invite Session UI (Invite Friends)*/
	UFUNCTION(BlueprintCallable)
	void ShowInviteUI();

	/*Login Voice*/
	UFUNCTION(BlueprintCallable)
	void VoiceLogin();

	/*Login Voice*/
	UFUNCTION(BlueprintCallable)
	void VoiceReleaseUser();

	UFUNCTION(BlueprintCallable)
	void VoiceTest();

	UFUNCTION(BlueprintCallable)
	/*Login to EOS */
	void Login();

	/*Logout to EOS */
	UFUNCTION(BlueprintCallable)
	void Logout();

protected:
	/*EOS Login check*/
	bool bIsLoggedIn;



};
