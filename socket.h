#ifndef __SOCKET_H_
#define __SOCKET_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define DEFAULT_PORT "27015"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>

class Socket {
private:

public:
	Socket(std::string ip, std::string msg = "hello world ") {	
		WSADATA wsaData;
		int iResult;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) {
		    printf("WSAStartup failed: %d\n", iResult);
		}

		struct addrinfo *result = NULL,
		                *ptr = NULL,
		                hints;

		printf("%s\n", ip.c_str());
		ZeroMemory( &hints, sizeof(hints) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(ip.c_str(), DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
		    printf("getaddrinfo failed: %d\n", iResult);
		    WSACleanup();
		}

		SOCKET ConnectSocket = INVALID_SOCKET;
		// Attempt to connect to the first address returned by
		// the call to getaddrinfo
		ptr=result;

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
		    ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
		    printf("Error at socket(): %ld\n", WSAGetLastError());
		    freeaddrinfo(result);
		    WSACleanup();
		}

		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
		    closesocket(ConnectSocket);
		    ConnectSocket = INVALID_SOCKET;
		}

		// Should really try the next address returned by getaddrinfo
		// if the connect call failed
		// But for this simple example we just free the resources
		// returned by getaddrinfo and print an error message

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
		    printf("Unable to connect to server!\n");
		    WSACleanup();
		}

		#define DEFAULT_BUFLEN 512

		int recvbuflen = DEFAULT_BUFLEN;

		char *sendbuf = (char*)msg.c_str();
		char recvbuf[DEFAULT_BUFLEN];

		// Send an initial buffer
		iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR) {
		    printf("send failed: %d\n", WSAGetLastError());
		    closesocket(ConnectSocket);
		    WSACleanup();
		}

		printf("Bytes Sent: %ld\n", iResult);

		iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
		    printf("shutdown failed: %d\n", WSAGetLastError());
		    closesocket(ConnectSocket);
		    WSACleanup();
		}

		do {
		    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		    if (iResult > 0) {
		        printf("Bytes received: %d\n", iResult);
		        recvbuf[iResult] = '\0';
		    } else if (iResult == 0) {
		        printf("Connection closed\n");
		    } else {
		        printf("recv failed: %d\n", WSAGetLastError());
		    }
		    printf("%s\n", recvbuf);
		} while (iResult > 0);
	};
	~Socket() {
	};
};

#endif