// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MINIDS_API PacketSession : public TSharedFromThis<PacketSession>
{
public:
	PacketSession(class FSocket* Socket);
	~PacketSession();

	void Run();

	void Disconnect();

public:
	class FSocket* Socket;
};
