#pragma once

#include "Types.h"
#include "Macro.h"
#include "Global.h"
#include "TLS.h"

#define WIN32_LEAN_AND_MEAN // 거의 사용하지 않는 API를 제외하여 Win32 헤더 파일의 크기를 줄입니다.

#include <iostream>
using namespace std;

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include <vector>
#include <set>
#include <queue>

#include <thread>
#include <mutex>

#include "Lock.h"
#include "SendBuffer.h"
