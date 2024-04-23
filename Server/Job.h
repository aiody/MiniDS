#pragma once
#include <functional>
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"

class Job
{
	using FuncType = function<void()>;

public:
	Job(FuncType func) : _func(func) {}

	template<typename T, typename Ret, typename... Args>
	Job(shared_ptr<T> owner, Ret(T::*memFunc)(Args...), Args... args)
	{
		_func = [owner, memFunc, args...]()
			{
				(owner.get()->*memFunc)(args...);
			};
	}

	void Execute()
	{
		_func();
	}

private:
	FuncType _func;
};
