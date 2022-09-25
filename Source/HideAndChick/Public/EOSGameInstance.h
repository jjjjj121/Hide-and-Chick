// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "EOSGameInstance.generated.h"

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

protected:
	/*Initialize*/
	virtual void Init() override;

	/*Login to EOS */
	void Login();

	/*Bind On Login Complete (EOS)*/
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	/*Bind On Create Session Complete */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/*Bind On Destroy Session Complete */
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/*Bind On Get All Friends Complete */
	void OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

protected:
	/*Create Session*/
	UFUNCTION(BlueprintCallable)
	void CreateSession();

	/*Destroy Session*/
	UFUNCTION(BlueprintCallable)
	void DestorySession();

	/*Get Friends List*/
	UFUNCTION(BlueprintCallable)
	void GetAllFriends();

protected:
	/*EOS Login check*/
	bool bIsLoggedIn;



};
