#include "server.h"
#include <string>
#include <iostream>
#include <map>
#include <sstream>

int main() {
	std::map< std::string, std::string > m;
	Server S;
	S.setCallback([&](std::string s, Client c) -> int {
		std::stringstream ss(s);
		std::string s2;
		ss >> s2;
		if (s2 == "set") {
			std::string x, y;
			ss >> x >> y;
			m[x] = y;
		} else if (s2 == "get") {
			std::string x;
			ss >> x;
			S.sendto(c, m[x]);
		}
	});
	getchar();
	return 0;	
}	