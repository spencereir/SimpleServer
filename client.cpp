#include "socket.h"
#include <string>
#include <iostream>
int main(int argc, char *argv[]) {
	std::string s(argv[1]);
	std::string msg;
	for (int i = 2; i < argc; i++) {
		msg += std::string(argv[i]) + " ";
	}
	std::cout << s << std::endl;
	Socket S(s, msg);
	return 0;
}