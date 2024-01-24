#pragma once

#include "Types.h"
#include "Macro.h"
#include "Global.h"

#define WIN32_LEAN_AND_MEAN // 거의 사용하지 않는 API를 제외하여 Win32 헤더 파일의 크기를 줄입니다.

#include <iostream>
using namespace std;
#include <thread>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include <vector>
