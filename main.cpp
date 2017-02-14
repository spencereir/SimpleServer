#include "server.h"
#include <string>
#include <iostream>

int main() {
	Server S;
	S.setCallback([&](std::string s) -> int {
		S.broadcast(s);	
	});
	getchar();
	return 0;	
}	