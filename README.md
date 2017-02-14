# SimpleServer
Simple C++ server for Windows using Winsock 2 and threading

## Setting up a Server

Servers are run by default on port 27015 (config to be added later). The Server class takes a parameter of type `int (*f)(std::string)`; a callback function that is passed received messages.

Here is an example server that prints any message it receives.

``` cpp
#include "server.h"
#include <iostream>
#include <string>

int main() {
  Server S([](std::string s) -> int {
      std::cout << s << std::endl;
  });
}
```

Additionally, `sendto` and `broadcast` functions are supported, for sending messages to targeted clients or all clients. It will be implemented soon to allow these functions to be called from within the callback.

## Setting up a Client

Client support is not integrated terribly well yet.