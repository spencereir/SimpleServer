#include "server.h"
#include <string>
#include <iostream>

int main() {
	Server S;
	S.setCallback([&](std::string s) -> int {
		std::cout << s << std::endl;
	});
	getchar();
	return 0;	
}	