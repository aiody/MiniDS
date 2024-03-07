// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/ArrayWriter.h"

struct PacketHeader
{
	int16 size;
	int16 id;

	PacketHeader() : size(0), id(0) { }
	PacketHeader(int16 size, int16 id) : size(size), id(id) { }
	friend FArchive& operator<<(FArchive& Ar, PacketHeader& header)
	{
		Ar << header.size << header.id;
		return Ar;
	}
};

/**
 * 
 */
class MINIDS_API ClientSocket2
{
public:
	ClientSocket2();
	~ClientSocket2();

	static ClientSocket2* Instance()
	{
		static ClientSocket2 instance;
		return &instance;
	}

	bool InitSocket();
	void DeleteSocket();
	bool Connect(FString ip, uint16 port);
	void Disconnect();
	void Recv();
	void Send(TSharedRef<FArrayWriter> buffer);

	void Dispatch();

private:
	FSocket* _socket = nullptr;
};
