#pragma once

#define OUT

/*----------------
	   LOCK
----------------*/

#define USE_MANY_LOCKS(count)		Lock _locks[count];
#define USE_LOCK					USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(idx)			ReadLockGuard readLockGuard_##idx(_locks[idx]);
#define READ_LOCK					READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)			WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#define WRITE_LOCK					WRITE_LOCK_IDX(0)

/*-----------------
	   CRASH
-----------------*/
/*
__analysis_assume
crash가 nullptr이면 Code Analysis 실행시 경고로 잡힐 수도 있다.
CRASH는 의도적으로 에러를 만들기 위한 코드이기 때문에 crash가 nullptr인 것은 필요한 코드이다.
그래서 crash가 nullptr이 아니라고 Code Analysis에게 힌트를 줘서 경고로 잡히지 않도록 하는 것이다.
*/
#define CRASH(cause)							\
{												\
	int32* crash = nullptr;						\
	_Analysis_assume_(crash != nullptr);		\
	*crash = 0xDEADBEEF;						\
}												\

#define ASSERT_CRASH(expr)						\
{												\
	if (!(expr))								\
		CRASH("ASSERT_CRASH");					\
}												\
