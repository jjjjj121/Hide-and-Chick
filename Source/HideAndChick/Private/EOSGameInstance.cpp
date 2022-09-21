// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"

//#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"



UEOSGameInstance::UEOSGameInstance()
{
}

void UEOSGameInstance::Init()
{
	Super::Init();

	/*Use Online Subsystem*/
	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		/*Set Session Interface*/
		if (SessionInterface = OnlineSubsystem->GetSessionInterface())
		{



		}
	}

}


