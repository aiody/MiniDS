#pragma once

#include "Types.h"
#include "Macro.h"
#include "Global.h"
#include "TLS.h"

#include <iostream>
using namespace std;

#define NOMINMAX
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <vector>
#include <set>
#include <queue>
#include <map>

#include <thread>
#include <mutex>

#include "Lock.h"
#include "SendBuffer.h"
#include "Session.h"

#include "Job.h"
#include "JobQueue.h"
#include "JobTimer.h"
