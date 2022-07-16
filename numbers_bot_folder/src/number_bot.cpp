//
// Created by Александр Шоршин on 16.07.2022.
//

#include "Client.hpp"
#include <string>

int main(int argc, char **argv){


	if (argc < 3){
		std::cout << "Format: ./numbers_bot <port> <password>\n";
		return 1;
	}

	Client client("127.0.0.1", atoi(argv[1]), argv[2]);
	try {
		client.init();
		client.run();
	}
	catch (std::runtime_error &ex){
		std::cout << ex.what() << std::endl;
	}

	return 0;
}