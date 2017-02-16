# SimpleServer
Simple C++ server for Windows using Winsock 2 and threading

## Installing / Compiling

Download the repository and run `make` to generate the `libsimpleserver.a` library.

Compile by including the `include/` and `bin/` directories, as well as linking with `-lsimpleserver -lws2_32`

## Setting up a Server

Servers are run by default on port 27015 (config to be added later). The Server class takes a parameter of type `std::function<int(std::string)>`; a callback function that is passed received messages.

Here is an example server that prints any message it receives.

``` cpp
#include "server.h"
#include <iostream>
#include <string>

int main() {
  Server S([](std::string s) -> int {
      std::cout << s << std::endl;
  });
  getchar();  // wait for program termination
}
```

Additionally, `sendto` and `broadcast` functions are supported, for sending messages to targeted clients or all clients. Here is how you can access them within the callback function; we will build a server that echoes back messages to clients.

``` cpp
#include "server.h"
#include <iostream>
#include <string>

int main() {
  Server S;
  S.setCallback([S](std::string s) -> int {     // Note here we can capture S to call the broadcast function
      S.broadcast(s);
  });
  getchar();
}
```

## Setting up a Client

Clients support the same style of operations that servers do. Clients create _sockets_ used to interface with a server. A socket can take either one or two parameters; the first is always the IP of the server to connect to, and the (optional) second parameter is the callback when a message is received. This callback can be set later (to capture the socket) by the `Socket::setCallback` method.

Clients also support the `Socket::send` method, which sends a string to the server

## Sample App: Chat room

Using these libraries, we can implement a simple chat room in less than 35 lines of code; most of it boilerplate.

``` cpp
// main.cpp
#include "server.h"
#include <string>
#include <iostream>

int main() {
	Server S;
	S.setCallback([&](std::string s) -> int {
		// when a message is read, format it and send to all clients
		auto first_space = s.find(' ');
		S.broadcast(s.substr(0, first_space) + ">" + s.substr(first_space, s.length()));
		std::cout << s << std::endl;
	});
	getchar();
	return 0;	
}	
```

``` cpp
// client.cpp
#include "socket.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
	std::string ip(argv[1]);
	std::string usr(argv[2]);
	Socket S(ip);
	S.setCallback([usr](std::string s) -> int {
		if (s.substr(0, usr.length()) != usr) {	// if a message is received from someone else, display it
			std::cout << s << std::endl;
		}
	});
	std::string s;
	while (getline(std::cin, s)) {				// continuously read messages; send to server
		S.send(usr + " " + s);
	}
	return 0;
}
```