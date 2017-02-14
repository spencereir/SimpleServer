#include "server.h"
#include <string>
#include <iostream>

int main() {
	Server S;
	S.setCallback([&](std::string s) -> int {
		auto first_space = s.find(' ');
		S.broadcast(s.substr(0, first_space) + ">" + s.substr(first_space, s.length()));
		std::cout << s << std::endl;
	});
	getchar();
	return 0;	
}	