//================= Copyright kotofyt, All rights reserved ==================//
// Purpose:
//===========================================================================//

#ifndef TIER0_NETWORK_H
#define TIER0_NETWORK_H

#include "platform.h"
#ifdef __linux__
#include "arpa/inet.h"
#endif
#ifdef __WIN32__
#include "winsock2.h"
#include "ws2tcpip.h"
#endif


PLATFORM_INTERFACE void Net_Init();
PLATFORM_INTERFACE void Net_Deinit();

#endif
