// Copyright Epic Games, Inc. All Rights Reserved.


#include "MiniDSGameModeBase.h"
#include "PaperCharacter.h"
#include "ClientSocket2.h"
#include "ServerPacketHandler.h"

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

	ServerPacketHandler::Init();

	Socket2 = ClientSocket2::Instance();
	if (Socket2->InitSocket())
	{
		if (Socket2->Connect(TEXT("127.0.0.1"), 9999))
		{
			UE_LOG(LogClass, Log, TEXT("Server connected!!!"));

			Protocol::C_CHAT PktChat;
			PktChat.set_msg("Hello World!");
			TSharedRef<FArrayWriter> SendBuffer = ServerPacketHandler::MakeSendBuffer(PktChat);
			Socket2->Send(SendBuffer);

			DispatcherThread* Thread = DispatcherThread::Instance([this]()
				{
					Socket2->Dispatch();
				});
		}
		else
		{
			Socket2->DeleteSocket();
		}
	}

	//Socket = ClientSocket::Instance();
	//if (Socket->InitSocket())
	//{
	//	if (Socket->Connect("127.0.0.1", 9999))
	//	{
	//		UE_LOG(LogClass, Log, TEXT("IOCP Server connect success!"));
	//		// 서버 데이터 처리 쓰레드 시작
	//		DispatcherThread* Thread = DispatcherThread::Instance([this]()
	//			{
	//				Socket->Dispatch();
	//			});
	//
	//		//Socket->SendChatMessage("Hello World!");
	//	}
	//}
}
