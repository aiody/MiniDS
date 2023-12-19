// Fill out your copyright notice in the Description page of Project Settings.


#include "DispatcherThread.h"

DispatcherThread::DispatcherThread(std::function<void()> callback)
	: Callback(callback)
{
	Thread = FRunnableThread::Create(this, TEXT("DispatcherThread"));
}

DispatcherThread::~DispatcherThread()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

bool DispatcherThread::Init()
{
	return true;
}

uint32 DispatcherThread::Run()
{
	Callback();
	return 0;
}

void DispatcherThread::Stop()
{
}

void DispatcherThread::Exit()
{
}
