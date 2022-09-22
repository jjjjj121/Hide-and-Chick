// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"


#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"



UEOSGameInstance::UEOSGameInstance()
{
	bIsLoggedIn = false;
}

void UEOSGameInstance::Init()
{
	Super::Init();

	/*Use Online Subsystem*/
	OnlineSubsystem = IOnlineSubsystem::Get();
	
	/*자격증명*/
	//Identity = OnlineSubsystem->GetIdentityInterface();

	/*Login EOS*/
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
			UE_LOG(LogTemp, Warning, TEXT("Login EOS : %d"));
		}
	}
}

void UEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete : %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		/*If Create Session Complete Clear Bind*/
		SessionInterface->ClearOnCreateSessionCompleteDelegates(this);
	}

}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("OnLoginComplete : %d"), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Login Complete"));

	/*bool Is Login?*/
	bIsLoggedIn = bWasSuccessful;

	if (bWasSuccessful)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			/*If Longin EOS Complete Clear Bind*/
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}

	}

}



void UEOSGameInstance::CreateSession()
{

	

	/*EOS Login Check*/
	if (bIsLoggedIn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Session"));

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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Create Session : Not Logged In"));
	}
}


