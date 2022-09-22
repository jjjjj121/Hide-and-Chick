// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"



UEOSGameInstance::UEOSGameInstance()
{
}

void UEOSGameInstance::Init()
{
	Super::Init();

	/*Use Online Subsystem*/
	OnlineSubsystem = IOnlineSubsystem::Get();

	/*Bind Func*/
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSessionComplete);

}

void UEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete : %d"), bWasSuccessful);

	if (OnlineSubsystem)
	{
		/*Set Session Interface*/
		if (SessionInterface = OnlineSubsystem->GetSessionInterface())
		{
			SessionInterface->ClearOnCreateSessionCompleteDelegates(this);

		}
	}


}

void UEOSGameInstance::CreateSession()
{
	if (OnlineSubsystem)
	{
		/*Set Session Interface*/
		if (SessionInterface = OnlineSubsystem->GetSessionInterface())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsDedicated = false;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bIsLANMatch = true;
			SessionSettings.NumPublicConnections = 5;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bUsesPresence = true;

			//
			SessionInterface->CreateSession(0, FName("Create Session"), SessionSettings);


		}
	}
}


