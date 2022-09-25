// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"


#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "OnlineSessionSettings.h"



const FName HACSessionName = FName("HACSession");
	
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

		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnDestroySessionComplete);

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

void UEOSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete : %d"), bWasSuccessful);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Session Destroy Succeeded"));
	if (bWasSuccessful)
	{
		/*If Destroy Session Complete Clear Bind*/
		SessionInterface->ClearOnDestroySessionCompleteDelegates(this);
	}

}

void UEOSGameInstance::OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	if (bWasSuccessful)
	{
		/*Is Valid Online Subsystem*/
		if (OnlineSubsystem)
		{
			/*Is Valid FriendsInterface*/
			if (FriendsInterface.IsValid())
			{

				TArray<TSharedRef<FOnlineFriend>> FriendsList;
				/*Get Friends List */
				if (FriendsInterface->GetFriendsList(0, ListName, FriendsList))
				{
					
					for (TSharedRef<FOnlineFriend> Friend : FriendsList)
					{
						UE_LOG(LogTemp, Warning, TEXT("Get Friend!"));
						FString FriendName = Friend.Get().GetRealName();
						UE_LOG(LogTemp, Warning, TEXT("Friend : %s"), *FriendName);
					}

				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed Get Friends List"));
				}

			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Was Successful At Getting Friends List"));
	}
}

void UEOSGameInstance::OnFindeSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Find Session Complete : %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		/*Show Found Lobbies Num*/
		UE_LOG(LogTemp, Warning, TEXT("Found %d Lobbies"), SearchSettings->SearchResults.Num());
		
	}

	/*Is Valid Session Interface*/
	if (SessionInterface.IsValid())
	{
		/*Search Session*/
		SessionInterface->ClearOnFindSessionsCompleteDelegates(this);
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
		

		/*Is Valid Session Interface*/
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

			SessionInterface->CreateSession(0, HACSessionName, SessionSettings);

			UE_LOG(LogTemp, Warning, TEXT("HACSession")); 
		}
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Create Session : Not Logged In"));
	}
}

void UEOSGameInstance::FindSession()
{
	/*EOS Login Check*/
	if (bIsLoggedIn)
	{
		/*Is Valid Session Interface*/
		if (SessionInterface.IsValid())
		{
			/*Search Settings (Match Create Session Settings)*/
			SearchSettings = MakeShareable(new FOnlineSessionSearch());
			
			SearchSettings->MaxSearchResults = 1000;
			SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("HACLobby"), EOnlineComparisonOp::Equals);
			SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);


			/*bind Find Session Complete*/
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindeSessionComplete);

			/*Search Session*/
			SessionInterface->FindSessions(0, SearchSettings.ToSharedRef());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Find Session : Not Logged In"));
	}

}

void UEOSGameInstance::DestorySession()
{
	/*EOS Login Check*/
	if (bIsLoggedIn)
	{
		/*Is Valid Session Interface*/
		if (SessionInterface.IsValid())
		{
			SessionInterface->DestroySession(HACSessionName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Destroy Session : Not Logged In"));
	}

}

void UEOSGameInstance::GetAllFriends()
{
	/*EOS Login Check*/
	if (bIsLoggedIn)
	{
		/*Is Valid Online Subsystem*/
		if (OnlineSubsystem)
		{
			/*FriendsInterface*/
			if (FriendsInterface = OnlineSubsystem->GetFriendsInterface())
			{
				/*Read Friends List And Bind Complete Func*/
				FriendsInterface->ReadFriendsList(0, FString(""), FOnReadFriendsListComplete::CreateUObject(this, &UEOSGameInstance::OnGetAllFriendsComplete));
				
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Load Frinds List : Not Logged In"));
	}
}


