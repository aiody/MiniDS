// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketSession.h"

PacketSession::PacketSession(FSocket* Socket) : Socket(Socket)
{
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
}

void PacketSession::Disconnect()
{
}
