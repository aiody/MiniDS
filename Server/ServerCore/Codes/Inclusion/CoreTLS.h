#pragma once

extern thread_local uint32				LThreadId;
extern thread_local uint64				LEndTickCount;

extern thread_local class JobQueue*		LCurrentJobQueue; // 현재 실행중인 JobQueue가 있는지
