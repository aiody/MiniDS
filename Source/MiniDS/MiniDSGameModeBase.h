// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClientSocket.h"
#include "DispatcherThread.h"
#include "MiniDSGameModeBase.generated.h"

class APaperCharacter;

/**
 * 
 */
UCLASS()
class MINIDS_API AMiniDSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AMiniDSGameModeBase();

	virtual void StartPlay() override;

private:
	ClientSocket* Socket;
};
