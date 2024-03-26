#pragma once
#ifndef PSVR_CONNECTION
#define PSVR_CONNECTION

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

namespace psmovevr
{
	inline const char* DATA_BUFFER_VR = "{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|0|0|0|0|0|0|0|0|{}|{}|0|0|0|0|0|0|0|0";

	class Connection {
	public:
		SOCKET sock = INVALID_SOCKET;
		sockaddr_in remoteAddr;

		Connection();

		void start(unsigned int port);

		void send(const char* buffer);

		void stop();
	};
}
#endif
