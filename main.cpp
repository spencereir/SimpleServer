#include "server.h"
#include <string>
#include <iostream>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
 
int main() {
	Server S([](std::string s) -> int {
		std::cout << "RECEIVED: " <<  s << std::endl;
	});
	std::cout << "Broadcasting Hello" << std::endl;
	for (int i = 0; i < 10000; i++) {
		S.broadcast("Hello");
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	char x;
	std::cin >> x;
	return 0;	
}	