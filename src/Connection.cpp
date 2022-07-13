//
// Created by Александр Шоршин on 10.07.2022.
//

#include "Connection.hpp"

#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

int Connection::runCommand() {

	int answer;

	if (comlist.find(get_command()) != comlist.end()){
		answer = CALL_MEMBER_FN(*this, comlist.find(get_command())->second)();
	} else{
		Message message;
		message.set_who_code_whom_command_message("ircserv", "421",
												  !nickname.empty() ? nickname : "*",
												  "ERR_UNKNOWNCOMMAND <" + command_buff + ">",
												  "Unknown command");
		server->send_message(socket, message);
		answer = COM_NORMAL;
	}
	command_buff.clear();
	return answer;
}

void Connection::addLetterToBuff(char simbol) {
	command_buff.push_back(simbol);
}

std::string Connection::get_command_buff() const{
	return command_buff;
}

std::string Connection::get_command() {
	return command_buff.substr(0,command_buff.find(' '));
}


std::string Connection::get_nickname() const{
	return nickname;
}

bool Connection::check_authorized() const {
	if (!authorized) {
		Message message;
		message.set_who_code_whom_command_message("ircserv", "464", nickname,
												  "ERR_PASSWDMISMATCH", "Password incorrect");
		server->send_message(socket, message);
	}
	return authorized;
}

void Connection::send_start_massage() const {
	if (user_ref && !nickname.empty()) {
		Message message;
		message.set_who_code_whom_command_message("ircserv", "001", nickname,
												  "", "Welcome to IRC Network " + nickname + "!");
		server->send_message(socket, message);
	}
}

int Connection::func_exit() {
	return COM_EXIT;
}

int Connection::func_pass() {
	if (server->getPassword() == command_buff.substr(command_buff.find(' ') + 1)) {
		authorized = true;
	} else{
		Message message;
		message.set_who_code_whom_command_message("ircserv", "464", nickname,
												  "ERR_PASSWDMISMATCH", "Password incorrect");
		server->send_message(socket, message);
	}
	return COM_NORMAL;
}

int Connection::func_nick() {

	if (!check_authorized())
		return COM_NORMAL;

	Message message;

	if (command_buff.substr(command_buff.find(' ') + 1).empty()) {
		message.set_who_code_whom_command_message("ircserv", "431", nickname,
												  "ERR_NONICKNAMEGIVEN", "No nickname given");
		server->send_message(socket, message);
		return COM_NORMAL;
	}
	if (command_buff.substr(command_buff.find(' ') + 1) == nickname) {
		message.set_who_code_whom_command_message("ircserv", "436", nickname,
												  "ERR_NICKCOLLISION <" + nickname +">",
												  "Nickname collision KILL");
		server->send_message(socket, message);
		return COM_NORMAL;
	}
	if (database->add_nickname(command_buff.substr(command_buff.find(' ') + 1))) {
		if (!nickname.empty()) {
			database->delete_nickname(nickname);
			nickname = command_buff.substr(command_buff.find(' ') + 1);
		}
		else {
			nickname = command_buff.substr(command_buff.find(' ') + 1);
			send_start_massage();
		}
	} else {
		message.set_who_code_whom_command_message("ircserv", "433", nickname,
												  "ERR_NICKNAMEINUSE <" +
												  command_buff.substr(command_buff.find(' ') + 1) +">",
												  "Nickname is already in use");
		server->send_message(socket, message);
	}
	return COM_NORMAL;
}

int Connection::func_user() {

	if (!check_authorized())
		return COM_NORMAL;

	Message message;

	if (user_ref) {
		message.set_who_code_whom_command_message("ircserv", "462", nickname,
												  "ERR_ALREADYREGISTRED", "You may not reregister");
		server->send_message(socket, message);
		return COM_NORMAL;
	}

	std::string user, hostname, servername, realname;

	if (command_buff.find(':') != std::string::npos)
		realname = command_buff.substr(command_buff.find(':') + 1);
	else {
		message.set_who_code_whom_command_message("ircserv", "461", nickname,
												  "ERR_NEEDMOREPARAMS", "Not enough parameters");
		server->send_message(socket, message);
		return COM_NORMAL;
	}
	std::stringstream first_part(command_buff.substr(0, command_buff.find(':')));
	std::string parsed;
	if (!getline(first_part, parsed, ' ')) {
		message.set_who_code_whom_command_message("ircserv", "461", nickname,
												  "ERR_NEEDMOREPARAMS", "Not enough parameters");
		server->send_message(socket, message);
		return COM_NORMAL;
	}
	if (getline(first_part, parsed, ' '))
		user = parsed;
	else {
		message.set_who_code_whom_command_message("ircserv", "461", nickname,
												  "ERR_NEEDMOREPARAMS", "Not enough parameters");
		server->send_message(socket, message);
		return COM_NORMAL;
	}

	if (getline(first_part, parsed, ' '))
		hostname = parsed;
	else {
		message.set_who_code_whom_command_message("ircserv", "461", nickname,
												  "ERR_NEEDMOREPARAMS", "Not enough parameters");
		server->send_message(socket, message);
		return COM_NORMAL;
	}

	if (getline(first_part, parsed, ' '))
		servername = parsed;
	else {
		message.set_who_code_whom_command_message("ircserv", "461", nickname,
												  "ERR_NEEDMOREPARAMS", "Not enough parameters");
		server->send_message(socket, message);
		return COM_NORMAL;
	}

	if (getline(first_part, parsed, ' ')) {
		message.set_who_code_whom_command_message("ircserv", "461", nickname,
												  "ERR_NEEDMOREPARAMS", "Not enough parameters");
		server->send_message(socket, message);
		return COM_NORMAL;
	}

	user_ref = database->add_user(user, hostname, servername, realname);
	if (!user_ref) {
		message.set_who_code_whom_command_message("ircserv", "462", nickname,
												  "ERR_ALREADYREGISTRED", "You may not reregister");
		server->send_message(socket, message);
		return COM_NORMAL;
	}

	send_start_massage();
	return COM_NORMAL;
}

int Connection::func_pong() {

	if (!check_authorized())
		return COM_NORMAL;

	return COM_NORMAL;
}

int Connection::func_quit() {

	if (!check_authorized())
		return COM_NORMAL;

	return COM_QUIT;
}

int Connection::func_msg() {

	if (!check_authorized())
		return COM_NORMAL;

	std::stringstream first_part(command_buff.substr(0, command_buff.find(':')));
	std::string parsed;

	if (!getline(first_part, parsed, ' ')) {
		Message message;
		message.set_who_code_whom_command_message("ircserv", "461", nickname,
												  "ERR_NEEDMOREPARAMS <" + command_buff + ">",
												  "Not enough parameters");
		server->send_message(socket, message);
		return COM_NORMAL;
	}
	return COM_NORMAL;
}

int Connection::func_join() {

	if (!check_authorized())
		return COM_NORMAL;

	return COM_NORMAL;
}

int Connection::func_part() {

	if (!check_authorized())
		return COM_NORMAL;

	return COM_NORMAL;
}


