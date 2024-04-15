// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSocket2.h"

#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Serialization/ArrayReader.h"
#include "ServerPacketHandler.h"

#include "Protocol/Protocol.pb.h"

ClientSocket2::ClientSocket2()
{
}

ClientSocket2::~ClientSocket2()
{
}

bool ClientSocket2::InitSocket()
{
	if (_socket == nullptr)
	{
		_socket = FTcpSocketBuilder(TEXT("Client Socket"))
			.AsBlocking()
			.AsReusable()
			.BoundToEndpoint(FIPv4Endpoint::Any);
	}

	return (_socket != nullptr);
}

void ClientSocket2::DeleteSocket()
{
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(_socket);
	_socket = nullptr;
}

bool ClientSocket2::Connect(FString ip, uint16 port)
{
	FIPv4Address address;
	if (FIPv4Address::Parse(ip, address) == false)
		return false;
	FIPv4Endpoint endPoint(address, port);

	bool result = _socket->Connect(endPoint.ToInternetAddr().Get());
	return result;
}

void ClientSocket2::Disconnect()
{
	if (_socket != nullptr)
	{
		_socket->Shutdown(ESocketShutdownMode::ReadWrite);
		_socket->Close();
	}
}

void ClientSocket2::Recv()
{
	// Read Header
	TArray<uint8> HeaderBuffer;
	uint16 HeaderSize = sizeof(PacketHeader);
	HeaderBuffer.AddZeroed(HeaderSize);
	int32 outBytesRead = 0;
	if (false == _socket->Recv(HeaderBuffer.GetData(), HeaderBuffer.Num(), outBytesRead))
	{
		ESocketErrors LastErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();
		UE_LOG(LogClass, Error, TEXT("Recv Header failed with error code (%d) error (%s)"), LastErr, ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(LastErr));
	}

	if (HeaderSize != outBytesRead)
	{
		UE_LOG(LogClass, Error, TEXT("Recv Header failed."));
		return;
	}

	PacketHeader* header = reinterpret_cast<PacketHeader*>(HeaderBuffer.GetData());

	UE_LOG(LogClass, Log, TEXT("Recv Header = Id : %d,  Size : %d"), header->id, header->size);

	// Read Payload
	int32 PayloadSize = header->size - HeaderSize;
	TArray<uint8> PayloadBuffer;
	PayloadBuffer.AddZeroed(PayloadSize);
	if (false == _socket->Recv(PayloadBuffer.GetData(), PayloadSize, outBytesRead))
	{
		ESocketErrors LastErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();
		UE_LOG(LogClass, Error, TEXT("Recv Payload failed with error code (%d) error (%s)"), LastErr, ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(LastErr));
	}

	if (PayloadSize != outBytesRead)
	{
		UE_LOG(LogClass, Error, TEXT("Recv Payload failed."));
		return;
	}

	FArrayWriter Buffer;
	Buffer.Serialize(HeaderBuffer.GetData(), HeaderBuffer.Num());
	Buffer.Serialize(PayloadBuffer.GetData(), PayloadBuffer.Num());
	ServerPacketHandler::HandlePacket(Buffer.GetData(), Buffer.Num());
}

void ClientSocket2::Send(TSharedRef<FArrayWriter> buffer)
{
	int32 outBytesSent = 0;
	if (false == _socket->Send(buffer->GetData(), buffer->Num(), outBytesSent))
	{
		ESocketErrors LastErr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();
		UE_LOG(LogClass, Error, TEXT("Send failed with error code (%d) error (%s)"), LastErr, ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetSocketError(LastErr));
	}
}

void ClientSocket2::Dispatch()
{
	Recv();
}
