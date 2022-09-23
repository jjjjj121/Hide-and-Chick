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
	Identity = OnlineSubsystem->GetIdentityInterface();
	
	if (Identity.IsValid())
	{
		/*Bind Login Complete*/
		Identity->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::OnLoginComplete);
		if (Identity->OnLoginCompleteDelegates->IsBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("Bind Succeeeded"));

			/*Login EOS*/
			Login();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Bind Failed"));
		}

		
	}


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
		if (Identity.IsValid())
		{
			//계정 자격 증명(개발자 전용)
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString("127.0.0.1:8081");
			Credentials.Token = FString("HACCredential");
			Credentials.Type = FString("developer");

			////계정 자격 증명
			//FOnlineAccountCredentials Credentials;
			//Credentials.Id = FString("");
			//Credentials.Token = FString("");
			//Credentials.Type = FString("accountportal");


			Identity->Login(0, Credentials);
			UE_LOG(LogTemp, Warning, TEXT("Login EOS : %d"));
		}
	}
}

void UEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete : %d"), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Session Create Succeeded"));
	if (bWasSuccessful)
	{
		/*If Create Session Complete Clear Bind*/
		SessionInterface->ClearOnCreateSessionCompleteDelegates(this);
	}

}


void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Login Complete"));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Login Complete"));

	/*bool Is Login?*/
	bIsLoggedIn = bWasSuccessful;

	if (bWasSuccessful)
	{
		if (Identity.IsValid())
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
		

		/*Set Session Interface*/
		if (SessionInterface.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsDedicated = false;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bIsLANMatch = false;
			SessionSettings.NumPublicConnections = 5;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bUseLobbiesIfAvailable = true;

			SessionSettings.Set(SEARCH_KEYWORDS, FString("HACLobby"), EOnlineDataAdvertisementType::ViaOnlineService);

			SessionInterface->CreateSession(0, FName("Create Session"), SessionSettings);

			UE_LOG(LogTemp, Warning, TEXT("Create Session"));
		}
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Create Session : Not Logged In"));
	}
}


