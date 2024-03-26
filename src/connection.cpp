#include "connection.h"

psmovevr::Connection::Connection() {}

void psmovevr::Connection::start(unsigned int port) {
	// this shit is black magic
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		exit(1);
		return;
	}

	sock = INVALID_SOCKET;
	if ((sock = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cerr << "Failed the socket with error: " << WSAGetLastError() << '\n';
		exit(1);
		return;
	}

	u_long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);

	// this too
	ZeroMemory(&remoteAddr, sizeof(remoteAddr));
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &remoteAddr.sin_addr);

	connect(sock, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr));
	std::cout << WSAGetLastError() << std::endl;
}

void psmovevr::Connection::send(const char* buffer) {
	int sent = ::send(sock, buffer, strlen(buffer), 0); 
	if (sent == SOCKET_ERROR)
	{
		std::cerr << "Failed the sendto with error: " << WSAGetLastError() << '\n';
		exit(1);
		return;
	}
}

void psmovevr::Connection::stop() {
	::closesocket(sock);
	WSACleanup();
}
