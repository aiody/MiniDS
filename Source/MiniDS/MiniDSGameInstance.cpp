// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniDSGameInstance.h"

#include "ClientSocket2.h"
#include "DispatcherThread.h"

#include "PaperCharacter.h"

void UMiniDSGameInstance::ConnectToGameServer()
{
	Socket = ClientSocket2::Instance();
	Socket->InitSocket();

	if (Socket->Connect(TEXT("127.0.0.1"), 9999))
	{
		UE_LOG(LogClass, Log, TEXT("Server connected!!!"));

		//Protocol::C_CHAT PktChat;
		//PktChat.set_msg("Hello World!");
		//TSharedRef<FArrayWriter> SendBuffer = ServerPacketHandler::MakeSendBuffer(PktChat);
		//Socket->Send(SendBuffer);

		Thread = DispatcherThread::Instance([this]()
			{
				Socket->Dispatch();
			});
	}
	else
	{
		Socket->DeleteSocket();
	}
}

void UMiniDSGameInstance::DisconnectFromGameServer()
{
	Socket->Disconnect();
	if (Thread != nullptr)
		Thread->Stop();
	Socket->DeleteSocket();
}

void UMiniDSGameInstance::SpawnWeber()
{
	FVector SpawnLocation(0, 0, 100);
	FRotator SpawnRotation(0, 0, 0);
	
	auto* World = GetWorld();
	if (World == nullptr)
		return;
	
	//World->SpawnActor<APaperCharacter>(Weber, SpawnLocation, SpawnRotation);
	World->SpawnActor(Weber, &SpawnLocation);
}
