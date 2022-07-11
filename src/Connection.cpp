//
// Created by Александр Шоршин on 10.07.2022.
//

#include "Connection.hpp"

#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

std::string Connection::runCommand() {

	std::string answer;

	(void )authorized;
//	if (!authorized)
//		return "464 ERR_PASSWDMISMATCH\n";
	if (comlist.find(get_command()) != comlist.end()){
		answer = CALL_MEMBER_FN(*this, comlist.find(get_command())->second)();
	} else{
		answer += "421 ERR_UNKNOWNCOMMAND <" + command_buff + "> :Unknown command\n";
	}
	command_buff.clear();
	return answer;
}

void Connection::addLetterToBuff(char simbol) {
	command_buff += simbol;
}

std::string Connection::get_command_buff() const{
	return command_buff;
}

std::string Connection::get_command() {
	return command_buff.substr(0,command_buff.find(' '));
}

std::string Connection::func_exit() {
	return "EXIT";
}

std::string Connection::func_pass() {
	if (server->getPassword() == command_buff.substr(command_buff.find(' ') + 1, command_buff.length())) {
		authorized = true;
		return "";
	} else
		return "464 ERR_PASSWDMISMATCH: Password incorrect\n";
}

