#include "server.h"

void Server::listener(Client c) {
	int iResult = 1;
	char recvbuf[512];
	do {
	    iResult = recv(c.sock, recvbuf, 512, 0);
	    if (iResult > 0) {
	        printf("Bytes received: %d\n", iResult);
	        callback(std::string(recvbuf));	
	    } else if (iResult == 0) {
	        printf("Connection closing...\n");
	    } else {
	        printf("recv failed: %d\n", WSAGetLastError());
			clients.erase(c);
			closesocket(c.sock);
			listeners.erase(c.id);
	    }
	} while (!done && iResult > 0);		
}

void Server::acceptClients() {
	do {
		SOCKET ClientSocket = INVALID_SOCKET;
		ClientSocket = accept(ListenSocket, NULL, NULL);

		if (ClientSocket != INVALID_SOCKET) {
			Client newClient = {client_id++, ClientSocket};
			clients.insert(newClient);
			listeners[newClient.id] = std::thread(listener, this, newClient);
			sendto(newClient, "Connection accepted");
		}
	} while (!done);
}

int Server::sendto(Client c, std::string s) {
	int res = send(c.sock, s.c_str(), s.length(), 0);
	if (res == SOCKET_ERROR) {
		clients.erase(c);
		closesocket(c.sock);
		printf("send failed: %d\n", WSAGetLastError());
	}
	return res;
}

void Server::broadcast(std::string s) {
	for (auto c : clients) {
		int res = sendto(c, s);
	}
}

void Server::setCallback(std::function< int (std::string)> _callback) {
	callback = _callback;
}

Server::Server() {
	int iResult;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
	    printf("WSAStartup failed: %d\n", iResult);
	}
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
	    printf("getaddrinfo failed: %d\n", iResult);
	    WSACleanup();
	}
	ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
	    printf("Error at socket(): %ld\n", WSAGetLastError());
	    freeaddrinfo(result);
	    WSACleanup();
	}
	   
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
    }
    freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
	    printf("Listen failed with error: %ld\n", WSAGetLastError());
	    closesocket(ListenSocket);
	    WSACleanup();
	}

	printf("Starting thread\n");
	clientAccepter = std::thread(acceptClients, this);		
	clientAccepter.detach();
}

Server::Server(int (*_callback)(std::string)) : callback(_callback) {
	int iResult;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
	    printf("WSAStartup failed: %d\n", iResult);
	}
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
	    printf("getaddrinfo failed: %d\n", iResult);
	    WSACleanup();
	}
	ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
	    printf("Error at socket(): %ld\n", WSAGetLastError());
	    freeaddrinfo(result);
	    WSACleanup();
	}
	   
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
    }
    freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
	    printf("Listen failed with error: %ld\n", WSAGetLastError());
	    closesocket(ListenSocket);
	    WSACleanup();
	}

	printf("Starting thread\n");
	clientAccepter = std::thread(acceptClients, this);		
	clientAccepter.detach();
}

Server::~Server() {
	done = true;
	clientAccepter.join();
	for (auto c : clients) {
		listeners[c.id].join();
		closesocket(c.sock);
	}
	WSACleanup();
}