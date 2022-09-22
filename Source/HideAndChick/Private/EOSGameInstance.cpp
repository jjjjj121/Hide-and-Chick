// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"



UEOSGameInstance::UEOSGameInstance()
{
}

void UEOSGameInstance::Init()
{
	Super::Init();

	/*Use Online Subsystem*/
	OnlineSubsystem = IOnlineSubsystem::Get();
	
	Login();

	if(OnlineSubsystem)
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();


		/*Bind Func*/
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSessionComplete);
	}
}

void UEOSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		//Login ID
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			//계정 자격 증명
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString();
			Credentials.Token = FString();
			Credentials.Type = FString("accountportal");



			Identity->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete : %d"), bWasSuccessful);

	if (bWasSuccessful)
	{

		SessionInterface->ClearOnCreateSessionCompleteDelegates(this);


	}

}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{

}

void UEOSGameInstance::CreateSession()
{

	UE_LOG(LogTemp, Warning, TEXT("CreateSession : %d"));

	/*Set Session Interface*/
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.bUsesPresence = true;



		SessionInterface->CreateSession(0, FName("Create Session"), SessionSettings);


	}

}


