#include "vr_connection.h"

vr::VRConnection::VRConnection(unsigned int port) {
	// this shit is black magic
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		exit(1);
		return;
	}
	
	sock = INVALID_SOCKET;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		std::cerr << "Failed the socket with error: " << WSAGetLastError() << '\n';
		exit(1);
		return;
	}

	// this too
	ZeroMemory(&remoteAddr, sizeof(remoteAddr));
	remoteAddr.sin_family = AF_INET;	
	remoteAddr.sin_port = htons(port);
	remoteAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void vr::VRConnection::send(const char* buffer) {
	int sent = sendto(sock, buffer, strlen(buffer), 0, (sockaddr*)&remoteAddr, sizeof(remoteAddr));
	if (sent == SOCKET_ERROR)
	{
		std::cerr << "Failed the sendto with error: " << WSAGetLastError() << '\n';
		exit(1);
		return;
	}
}

void vr::VRConnection::stop() {
	closesocket(sock);
	WSACleanup();
}
