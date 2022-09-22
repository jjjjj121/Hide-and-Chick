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
	//IOnlineIdentityPtr Identity;


protected:
	/*Initialize*/
	virtual void Init() override;

	/*Login to EOS */
	void Login();

	/*Bind On Create Session Complete */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/*Bind On Login Complete (EOS)*/
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

protected:
	/*Create Session*/
	UFUNCTION(BlueprintCallable)
	void CreateSession();


protected:
	/*EOS Login check*/
	bool bIsLoggedIn;



};
