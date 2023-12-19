// Copyright Epic Games, Inc. All Rights Reserved.


#include "MiniDSGameModeBase.h"
#include "PaperCharacter.h"

AMiniDSGameModeBase::AMiniDSGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APaperCharacter> BP_Char(TEXT("Blueprint'/Game/Blueprint/BP_Weber.BP_Weber_C'"));
	
	if (BP_Char.Succeeded())
	{
		DefaultPawnClass = BP_Char.Class;
	}
}

void AMiniDSGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (!GEngine) return;

	Socket = ClientSocket::Instance();
	if (Socket->InitSocket())
	{
		if (Socket->Connect("127.0.0.1", 9999))
		{
			UE_LOG(LogClass, Log, TEXT("IOCP Server connect success!"));
			// 서버 데이터 처리 쓰레드 시작
			DispatcherThread* Thread = DispatcherThread::Instance([this]()
				{
					Socket->Dispatch();
				});

			Socket->SendChatMessage("Hello World!");
		}
	}
}
