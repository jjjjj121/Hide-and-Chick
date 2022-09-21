// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
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



protected:
	/*Initialize*/
	virtual void Init() override;


protected:






};
