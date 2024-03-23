#pragma once

#include <Windows.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace psmovevr
{
	const std::string DATA_BUFFER = "{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|0|0|0|0|0|0|0|0|{}|{}|0|0|0|0|0|0|0|0";

	class Connection {
	public:
		SOCKET sock;
		sockaddr_in remoteAddr;

		Connection(unsigned int port);

		void send(const char* buffer);

		void stop();
	};
}