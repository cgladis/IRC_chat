//
// Created by Commando Gladis on 7/5/22.
//

#include "Server.hpp"
#include <string>

int main(int argc, char **argv){
	if (argc < 3){
		std::cout << "Format: ./ircserv <port> <password>\n";
		return 1;
	}

	Server server(atoi(argv[1]));
	try {
		server.init();
		std::cout << server << std::endl;
		server.run();
	}
	catch (std::runtime_error &ex){
		std::cout << ex.what() << std::endl;
	}

    return 0;
}