// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"


#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Interfaces/VoiceInterface.h"
#include "OnlineSubsystemUtils.h"
//#include "OnlineSubsystemEOS.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"


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
			UE_LOG(LogTemp, Warning, TEXT("Try Login EOS"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Bind Failed"));
		}


	}


	if (OnlineSubsystem)
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
			////계정 자격 증명(개발자 전용)
			//FOnlineAccountCredentials Credentials;
			//Credentials.Id = FString("127.0.0.1:8081");
			//Credentials.Token = FString("HACCredential");
			//Credentials.Type = FString("developer");

			//계정 자격 증명
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString("");
			Credentials.Token = FString("");
			Credentials.Type = FString("accountportal");



			Identity->Login(0, Credentials);

		}
	}
}

void UEOSGameInstance::Logout()
{

	if (OnlineSubsystem)
	{
		//Login ID
		if (Identity.IsValid())
		{

			TSharedPtr<const FUniqueNetId> UserId5 = Identity->GetUniquePlayerId(0);
			if (Identity->GetLoginStatus(*UserId5))
			{
				UE_LOG(LogTemp, Warning, TEXT("State : Is Logged"));
				Identity->Logout(0);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Error ! State - Is Not Logged"));
			}
			
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

		GetWorld()->ServerTravel("/Game/HideandChick/Level/Lobby_Level?listen");
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

void UEOSGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Find Session Complete : %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		/*Show Found Lobbies Num*/
		UE_LOG(LogTemp, Warning, TEXT("Found %d Lobbies"), SearchSettings->SearchResults.Num());

		/*Is Valid Session Interface*/
		if (SessionInterface.IsValid())
		{

			TArray<FOnlineSessionSearchResult> SearchResults = SearchSettings->SearchResults;

			for (FOnlineSessionSearchResult Result : SearchResults)
			{
				FString Lobbies = Result.Session.OwningUserName;
				UE_LOG(LogTemp, Warning, TEXT("Session.OwningUsername : %s"), *Lobbies);
				Lobbies;
				UE_LOG(LogTemp, Warning, TEXT("Session.Info.ToString : %s"), *Lobbies);
				Lobbies = Result.GetSessionIdStr();
				UE_LOG(LogTemp, Warning, TEXT("SessionIdStr : %s"), *Lobbies);

			}



			if (SearchSettings->SearchResults.Num())
			{
				SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnJoinSessionComplete);
				SessionInterface->JoinSession(0, HACSessionName, SearchSettings->SearchResults[0]);
			}

		}


	}

	/*Is Valid Session Interface*/
	if (SessionInterface.IsValid())
	{
		/*Search Session*/
		SessionInterface->ClearOnFindSessionsCompleteDelegates(this);
	}

}

void UEOSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	/*Is Valid SessionInterface*/
	if (SessionInterface.IsValid())
	{
		FString ConnectionInfo = FString();
		SessionInterface->GetResolvedConnectString(SessionName, ConnectionInfo);
		if (!ConnectionInfo.IsEmpty())
		{
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				PC->ClientTravel(ConnectionInfo, TRAVEL_Absolute);

			}
		}
	}



}

void UEOSGameInstance::OnVoiceLoginComplete(const FString& PlayerName, const FVoiceChatResult& Result)
{
	if (Result.IsSuccess())
	{
		UE_LOG(LogTemp, Warning, TEXT("Voice Chat User Login Complete"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Voice Chat User Login Complete"));
		// You can now use this->VoiceChatUser to control the user's voice chat.
	}

}

void UEOSGameInstance::OnVoiceChatConnectComplete(const FVoiceChatResult& Result)
{

	if (Result.IsSuccess())
	{
		UE_LOG(LogTemp, Warning, TEXT("On Voice Chat Connect Complete"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On Voice Chat Connect Failed"));
	}

}





void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{

	/*bool Is Login?*/
	bIsLoggedIn = bWasSuccessful;
	const FUniqueNetId& LocalUserId = UserId;
	int32 UserNum = LocalUserNum;
	UE_LOG(LogTemp, Warning, TEXT("Get Unique PlayerId111 : %s"), &LocalUserId);
	UE_LOG(LogTemp, Warning, TEXT("Get Local User Num : %d"), UserNum);

	TSharedPtr<const FUniqueNetId> UserId2 = Identity->GetUniquePlayerId(0);
	Identity->GetUserAccount(*UserId2);
	UE_LOG(LogTemp, Warning, TEXT("Get Unique PlayerId222 : %s"), &UserId2);

	FString Nick = Identity->GetPlayerNickname(0);
	UE_LOG(LogTemp, Warning, TEXT("Get Local User NickName : %s"), &Nick);
	
	
	FPlatformUserId PlatformUserId = Identity->GetPlatformUserIdFromUniqueNetId(LocalUserId);
	UE_LOG(LogTemp, Warning, TEXT("Get platform User ID : %d"), PlatformUserId);
	UE_LOG(LogTemp, Warning, TEXT("Get platform User ID &&: %d"), &PlatformUserId);
	//UE_LOG(LogTemp, Warning, TEXT("Get platform User ID : %s"), PlatformUserId);
	UE_LOG(LogTemp, Warning, TEXT("Get platform User ID && : %s"), &PlatformUserId);
	FPlatformUserId PlatformUserId2 = Identity->GetPlatformUserIdFromUniqueNetId(*UserId2);
	UE_LOG(LogTemp, Warning, TEXT("Get platform Local User ID : %d"), PlatformUserId2);
	UE_LOG(LogTemp, Warning, TEXT("Get platform Local User ID &&: %d"), &PlatformUserId2);
	//UE_LOG(LogTemp, Warning, TEXT("Get platform Local User ID : %s"), PlatformUserId2);
	UE_LOG(LogTemp, Warning, TEXT("Get platform Local User ID &&: %s"), &PlatformUserId2);

	/*Identity -> UserAccount (사용자 계정 정보)*/
	TSharedPtr <FUserOnlineAccount> UserAccount = Identity->GetUserAccount(LocalUserId);
	UE_LOG(LogTemp, Warning, TEXT("Get  User Acount : %s"), &UserAccount);

	FUniqueNetIdRef UserID = UserAccount->GetUserId();
	UE_LOG(LogTemp, Warning, TEXT("Get  User Acount_ GetUserID: %s"), &UserID);

	FString RealName = UserAccount->GetRealName();
	UE_LOG(LogTemp, Warning, TEXT("Get  User Acount_ RealName : %s"), &RealName);

	FString AccessToken = UserAccount->GetAccessToken();
	UE_LOG(LogTemp, Warning, TEXT("Get  User Acount_ AccessToken : %s"), &AccessToken);



	if (bWasSuccessful)
	{

		UE_LOG(LogTemp, Warning, TEXT("Login Complete"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Login Complete"));


		if (Identity.IsValid())
		{
			/*If Longin EOS Complete Clear Bind*/
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}




	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Login Failed"));
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
			SessionSettings.NumPublicConnections = 10;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bUseLobbiesIfAvailable = true;
			SessionSettings.bUseLobbiesVoiceChatIfAvailable = true;
			SessionSettings.bAllowInvites = true;

			SessionSettings.Set(SEARCH_KEYWORDS, FString("HACLobby"), EOnlineDataAdvertisementType::ViaOnlineService);

			SessionInterface->CreateSession(0, HACSessionName, SessionSettings);






			UE_LOG(LogTemp, Warning, TEXT("HACSession"));


			//TSharedPtr<const FUniqueNetId> UserId33 = Identity->GetUniquePlayerId(0);
			//UE_LOG(LogTemp, Warning, TEXT("Get Unique PlayerId : %s"), &UserId33);
			//FPlatformUserId PlatformUserId = Identity->GetPlatformUserIdFromUniqueNetId(*UserId33);

			//VoiceChatUser->Login(PlatformUserId, UserId33->ToString(), TEXT(""), FOnVoiceChatLoginCompleteDelegate::CreateUObject(this, &UEOSGameInstance::OnVoiceLoginComplete));

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
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindSessionComplete);

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

void UEOSGameInstance::ShowUI()
{
	/*EOS Login Check*/
	if (bIsLoggedIn)
	{
		/*Is Valid Online Subsystem*/
		if (OnlineSubsystem)
		{
			if (UIInterface = OnlineSubsystem->GetExternalUIInterface())
			{
				UIInterface->ShowFriendsUI(0);
			}
		}
	}
}

void UEOSGameInstance::ShowInviteUI()
{
	/*EOS Login Check*/
	if (bIsLoggedIn)
	{
		/*Is Valid Online Subsystem*/
		if (OnlineSubsystem)
		{
			if (UIInterface = OnlineSubsystem->GetExternalUIInterface())
			{
				UIInterface->ShowInviteUI(0, HACSessionName);
			}
		}
	}
}


void UEOSGameInstance::VoiceLogin()
{
	//UE_LOG(LogTemp, Warning, TEXT("Try Voice Login"));

	//VoiceChat = IVoiceChat::Get();
	//VoiceChat->Initialize();
	//VoiceChat->Connect(FOnVoiceChatConnectCompleteDelegate::CreateUObject(this, &UEOSGameInstance::OnVoiceChatConnectComplete));
	//FString LoggedName = VoiceChat->GetLoggedInPlayerName();
	//
	//UE_LOG(LogTemp, Warning, TEXT("Logged In Player Name : %s"), &LoggedName);
	//

	//VoiceChatUser = VoiceChat->CreateUser();
	//
	//TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
	//UE_LOG(LogTemp, Warning, TEXT("Get Unique PlayerId : %s"), &UserId);
	//FPlatformUserId PlatformUserId = Identity->GetPlatformUserIdFromUniqueNetId(*UserId);
	//FString Name = UserId->ToString();
	//UE_LOG(LogTemp, Warning, TEXT("To String: %s"), &Name);
	//FString Name12 = UserId->ToString();
	//UE_LOG(LogTemp, Warning, TEXT("To String2 : %s"), &Name12);
	//FString WhatName= Identity->GetUniquePlayerId(0)->ToString();
	//UE_LOG(LogTemp, Warning, TEXT("Get To String : % s"), &WhatName);


	//if (Name == WhatName)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("The Same Name"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not Same Name"));
	//}


	//VoiceChatUser->Login(PlatformUserId, UserId->ToString(), TEXT(""), FOnVoiceChatLoginCompleteDelegate::CreateUObject(this, &UEOSGameInstance::OnVoiceLoginComplete));



	////////////////////////////////////////////////////////////////////////
	//TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
	//UE_LOG(LogTemp, Warning, TEXT("Get Unique PlayerId : %s"), &UserId);
	//FPlatformUserId PlatformUserId = Identity->GetPlatformUserIdFromUniqueNetId(*UserId);

	//VoiceChatUser->Login(PlatformUserId, UserId->ToString(), TEXT(""), FOnVoiceChatLoginCompleteDelegate::CreateUObject(this, &UEOSGameInstance::OnVoiceLoginComplete));
	////////////////////////////////////////////////////////////////////

	IVoiceChat* TestVoiceChat = IVoiceChat::Get();
	TestVoiceChat->Initialize();
	TestVoiceChat->Connect(FOnVoiceChatConnectCompleteDelegate::CreateUObject(this, &UEOSGameInstance::OnVoiceChatConnectComplete));



	this->VoiceChatUser = TestVoiceChat->CreateUser();
	

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(this->GetWorld());
	IOnlineIdentityPtr voiceIdentity = Subsystem->GetIdentityInterface();

	TSharedPtr<const FUniqueNetId> UserId = voiceIdentity->GetUniquePlayerId(0);
	FPlatformUserId PlatformUserId = voiceIdentity->GetPlatformUserIdFromUniqueNetId(*UserId);

	VoiceChatUser->Login(
		PlatformUserId,
		UserId->ToString(),
		TEXT(""),
		FOnVoiceChatLoginCompleteDelegate::CreateUObject(this, &UEOSGameInstance::OnVoiceLoginComplete));


}

void UEOSGameInstance::VoiceReleaseUser()
{
	VoiceChat = IVoiceChat::Get();
	if (this->VoiceChatUser != nullptr && VoiceChat != nullptr)
	{

		UE_LOG(LogTemp, Warning, TEXT("Release Voice Login"));
		VoiceChat->ReleaseUser(this->VoiceChatUser);
		this->VoiceChatUser = nullptr;
	}
}

void UEOSGameInstance::VoiceTest()
{
	VoiceChat = IVoiceChat::Get();
	VoiceChatUser = VoiceChat->CreateUser();
	//Identity->player


}
