#include "socket.h"
#include <string>
#include <iostream>
int main(int argc, char *argv[]) {
	std::string ip(argv[1]);
	std::string usr(argv[2]);
	Socket S(ip);
	S.setCallback([usr](std::string s) -> int {
		if (s.substr(0, usr.length()) != usr) {
			std::cout << s << std::endl;
		}
	});
	std::string s;
	while (getline(std::cin, s)) {
		S.send(usr + "> " + s);
	}
	return 0;
}