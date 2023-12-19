// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include <functional>

class ClientSocket;

class MINIDS_API DispatcherThread : public FRunnable
{
public:
	DispatcherThread(std::function<void()> callback);
	~DispatcherThread();

	static DispatcherThread* Instance(std::function<void()> callback)
	{
		static DispatcherThread instance(callback);
		return &instance;
	}

	// FRunnable override
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	FRunnableThread* Thread;
	std::function<void()> Callback;
};
