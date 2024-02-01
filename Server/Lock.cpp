#include "pch.h"
#include "Lock.h"

void Lock::WriteLock()
{
	const uint16 lockThreadId = (_lockFlag.load() & WRITE_FLAG_MASK) >> 16;
	if (lockThreadId == LThreadId)
	{
		_writeCount++;
		return;
	}

	const uint64 beginTick = ::GetTickCount64();
	const uint32 desired = (LThreadId << 16) & WRITE_FLAG_MASK;
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		::this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	if ((_lockFlag.load() & READ_FLAG_MASK) > 0)
		CRASH("INVALID_UNLOCK_ORDER");

	if (--_writeCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	const uint16 lockThreadId = (_lockFlag.load() & WRITE_FLAG_MASK) >> 16;
	if (lockThreadId == LThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	const uint64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_FLAG_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		::this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_FLAG_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}
