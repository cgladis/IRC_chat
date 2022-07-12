//
// Created by Александр Шоршин on 10.07.2022.
//

#include "Connection.hpp"

#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

Message Connection::runCommand() {

	Message answer;

	if (comlist.find(get_command()) != comlist.end()){
		answer = CALL_MEMBER_FN(*this, comlist.find(get_command())->second)();
	} else{
		answer = Message("421 ERR_UNKNOWNCOMMAND <" + command_buff + "> :Unknown command\n");
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


std::string Connection::get_nickname() const{
	return nickname;
}

Message Connection::func_exit() {
	return Message("EXIT");
}

Message Connection::func_pass() {
	if (server->getPassword() == command_buff.substr(command_buff.find(' ') + 1)) {
		authorized = true;
		return Message("");
	} else
		return Message("464 ERR_PASSWDMISMATCH : Password incorrect\n");
}

Message Connection::func_nick() {
	if (!authorized)
		return Message("464 ERR_PASSWDMISMATCH : Password incorrect\n");
	if (command_buff.substr(command_buff.find(' ') + 1).empty())
		return Message("431 ERR_NONICKNAMEGIVEN : No nickname given\n");
	if (command_buff.substr(command_buff.find(' ') + 1) == nickname)
		return Message("436 ERR_NICKCOLLISION <" + nickname + "> :Nickname collision KILL\n");
	if (database->add_nickname(command_buff.substr(command_buff.find(' ') + 1))) {
		if (!nickname.empty())
			database->delete_nickname(nickname);
		nickname = command_buff.substr(command_buff.find(' ') + 1);
		return Message("");
	} else
		return Message("433 ERR_NICKNAMEINUSE <" + command_buff.substr(command_buff.find(' ') + 1) + "> :Nickname is already in use\n");
}

Message Connection::func_user() {

	if (user_ref)
		return Message("462 ERR_ALREADYREGISTRED :You may not reregister\n");

	std::string user, hostname, servername, realname;

	if (command_buff.find(':') != std::string::npos)
		realname = command_buff.substr(command_buff.find(':') + 1);
	else
		return Message("461 ERR_NEEDMOREPARAMS <" + command_buff + "> :Not enough parameters\n");
	std::stringstream first_part(command_buff.substr(0, command_buff.find(':')));
	std::string parsed;
	if (!getline(first_part, parsed, ' '))
		return Message("461 ERR_NEEDMOREPARAMS <" + command_buff + "> :Not enough parameters\n");

	if (getline(first_part, parsed, ' '))
		user = parsed;
	else
		return Message("461 ERR_NEEDMOREPARAMS <" + command_buff + "> :Not enough parameters\n");

	if (getline(first_part, parsed, ' '))
		hostname = parsed;
	else
		return Message("461 ERR_NEEDMOREPARAMS <" + command_buff + "> :Not enough parameters\n");

	if (getline(first_part, parsed, ' '))
		servername = parsed;
	else
		return Message("461 ERR_NEEDMOREPARAMS <" + command_buff + "> :Not enough parameters\n");

	if (getline(first_part, parsed, ' '))
		return Message("461 ERR_NEEDMOREPARAMS <" + command_buff + "> :Not enough parameters\n");

	user_ref = database->add_user(user, hostname, servername, realname);
	if (!user_ref)
		return Message("462 ERR_ALREADYREGISTRED :You may not reregister\n");

	return Message(":ircserv 001 " + nickname + " :Welcome to IRC Network " + nickname + "!\n");
}

Message Connection::func_pong() {
	return Message("");
}

Message Connection::func_quit() {
	return Message("");
}

