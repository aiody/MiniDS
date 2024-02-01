#pragma once
#include <atomic>

/*
* Recursive Reader-Writer Spin Lock
* 
* 한 쓰레드만 Write Lock (Exclusive Lock)을 잡을 수 있다.
* 여러 쓰레드가 Read Lock (Shared Lock)을 잡을 수 있다.
* 한 쓰레드가 Write Lock을 잡고 있다면 다른 모든 쓰레드는 Write Lock도, Read Lock도 잡을 수 없다.
* 한 쓰레드가 Read Lock을 잡고 있다면 다른 쓰레드는 Read Lock만 잡을 수 있다.
*/
class Lock
{
	enum : uint32
	{
		MAX_SPIN_COUNT = 5000,
		ACQUIRE_TIMEOUT_TICK = 10000,
		EMPTY_FLAG = 0x0000'0000,
		WRITE_FLAG_MASK = 0xFFFF'0000,
		READ_FLAG_MASK = 0x0000'FFFF,
	};

public:
	/*
	* 재귀적 호출 정책 (한 쓰레드가 재귀적으로 호출할 때)
	* R -> R (0) : Read Flag를 하나 올림
	* W -> W (O) : _writeCount를 하나 올림
	* W -> R (O) : Write Lock을 잡은 쓰레드에 한해서 Read Lock을 잡을 수 있게 한다. 대신 해제할 때 R -> W 순서대로 해제하기.
	* R -> W (X) : Read Lock이 잡힌 상황에서는 다른 쓰레드도 Read Lock을 잡을 수 있는 상황이기 때문에 Write Lock 잡기는 불가능. (write-starvation 문제 발생 가능)
	*/
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	// _lockFlag : [WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
	//         W : WriteFlag (Write Lock을 잡은 Thread의 Id)
	//         R : ReadFlag (Read Lock Count)
	atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); };
	~ReadLockGuard() { _lock.ReadUnlock(); };
private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); };
	~WriteLockGuard() { _lock.WriteUnlock(); };
private:
	Lock& _lock;
};
