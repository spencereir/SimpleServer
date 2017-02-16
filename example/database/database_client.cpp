#include "socket.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
	Socket S(std::string(argv[1]), [](std::string s) -> int {
		std::cout << s << std::endl;
	});
	std::string s;
	while (std::cin >> s) {
		if (s == "get") {
			std::string k;
			std::cin >> k;
			S.send("get " + k);
		} else if (s == "quit") {
			break;
		} else {
			std::string a, b;
			std::cin >> a >> b;
			S.send("set " + a + " " + b);
		}
	}
}