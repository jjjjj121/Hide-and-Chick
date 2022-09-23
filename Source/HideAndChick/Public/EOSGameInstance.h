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


public:


public:
	/*Online Subsystem*/
	class IOnlineSubsystem* OnlineSubsystem;

public:
	/*Session Interface*/
	IOnlineSessionPtr SessionInterface;

	/*EOS Login Identity*/
	IOnlineIdentityPtr Identity;


public:
	/*Initialize*/
	virtual void Init() override;

	/*Login to EOS */
	void Login();

	/*Bind On Login Complete (EOS)*/
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	/*Bind On Create Session Complete */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);



public:
	/*Create Session*/
	UFUNCTION(BlueprintCallable)
	void CreateSession();

	
public:
	/*EOS Login check*/
	bool bIsLoggedIn;



};
