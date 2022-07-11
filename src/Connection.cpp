//
// Created by Александр Шоршин on 10.07.2022.
//

#include "Connection.hpp"

std::string Connection::runCommand() {

	if (!authorized)
		return "464";
	if (command_buff == "exit")
		return "EXIT";
	std::cout << command_buff << std::endl;
	command_buff.clear();
	return "";
}

void Connection::addLetterToBuff(char simbol) {
	command_buff += simbol;
}

std::string Connection::get_command_buff() const{
	return command_buff;
}

