// Fill out your copyright notice in the Description page of Project Settings.


#include "DispatcherThread.h"

DispatcherThread::DispatcherThread(std::function<void()> callback)
	: Callback(callback)
{
	Thread = FRunnableThread::Create(this, TEXT("DispatcherThread"));
}

DispatcherThread::~DispatcherThread()
{
	if (Thread != nullptr)
	{
		Thread->Kill();
		delete Thread;
	}
}

bool DispatcherThread::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dispatcher Thread Init!")));
	return true;
}

uint32 DispatcherThread::Run()
{
	while (Stopped.load() == false)
	{
		Callback();
	}
	return 0;
}

void DispatcherThread::Stop()
{
	Stopped.store(true);
}

void DispatcherThread::Exit()
{
	
}
