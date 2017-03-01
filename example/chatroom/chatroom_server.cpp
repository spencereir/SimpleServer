#include "server.h"
#include <string>
#include <iostream>

int main() {
	Server S;
	S.setCallback([&](std::string s, Client c) -> int {
		std::cout << s << std::endl;
	});
	getchar();
	//S.~Server();
	return 0;	
}	