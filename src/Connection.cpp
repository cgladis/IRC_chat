//
// Created by Александр Шоршин on 10.07.2022.
//

#include "Connection.hpp"

#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

int Connection::runCommand() {

	parse_command_buff();

	int answer;

	if (comlist.find(commands[0]) != comlist.end()){
		answer = CALL_MEMBER_FN(*this, comlist.find(commands[0])->second)();
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
	commands.clear();
	return answer;
}

void Connection::addLetterToBuff(char simbol) {
	command_buff.push_back(simbol);
}

std::string Connection::get_command_buff() const{
	return command_buff;
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
    if (authorized){
        Message message;
        message.set_who_code_whom_command_message("ircserv", "462", nickname,
                                                  "ERR_ALREADYREGISTRED",
                                                  "You may not reregister");
        server->send_message(socket, message);
        return COM_NORMAL;
    }
    if (commands.size() < 2) {
        Message message;
        message.set_who_code_whom_command_message("ircserv", "461", nickname,
                                                  "ERR_NEEDMOREPARAMS <" + commands[0] + ">",
                                                  "Not enough parameters");
        server->send_message(socket, message);
        return COM_NORMAL;
    }
	if (server->check_password(commands[1])) {
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

	if (commands.size() == 1) {
		message.set_who_code_whom_command_message("ircserv", "431", nickname,
												  "ERR_NONICKNAMEGIVEN", "No nickname given");
		server->send_message(socket, message);
		return COM_NORMAL;
	}
	if (commands[1] == nickname) {
		message.set_who_code_whom_command_message("ircserv", "436", nickname,
												  "ERR_NICKCOLLISION <" + nickname +">",
												  "Nickname collision KILL");
		server->send_message(socket, message);
		return COM_NORMAL;
	}
	if (database->add_nickname(commands[1])) {
		if (!nickname.empty()) {
			database->delete_nickname(nickname);
			nickname = commands[1];
		}
		else {
			nickname = commands[1];
			send_start_massage();
		}
	} else {
		message.set_who_code_whom_command_message("ircserv", "433", nickname,
												  "ERR_NICKNAMEINUSE <" + commands[1] +">",
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

    if (commands.size() != 5){
        message.set_who_code_whom_command_message("ircserv", "461", nickname,
                                                  "ERR_NEEDMOREPARAMS <" + commands[0] + ">",
                                                  "Not enough parameters");
        server->send_message(socket, message);
        return COM_NORMAL;
    }

	user_ref = database->add_user(commands[1], commands[2], commands[3], commands[4]);

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

	return COM_QUIT;
}

int Connection::func_msg() {

	if (!check_authorized())
		return COM_NORMAL;

	if (commands.size() != 3) {
		Message message;
		message.set_who_code_whom_command_message("ircserv", "461", nickname,
												  "ERR_NEEDMOREPARAMS <" + commands[0] + ">",
												  "Not enough parameters");
		server->send_message(socket, message);
		return COM_NORMAL;
	}
	return COM_NORMAL;
}

int Connection::func_join() {

	if (!check_authorized())
		return COM_NORMAL;

    if (commands.size() != 2){
        Message message;
        message.set_who_code_whom_command_message("ircserv", "461", nickname,
                                                  "ERR_NEEDMOREPARAMS <" + commands[0] + ">",
                                                  "Not enough parameters");
        server->send_message(socket, message);
        return COM_NORMAL;
    }

    Channel *channel = database->add_channel(commands[1]);
    channel->add_member(nickname);
    channels.insert(channel->get_name());

    {
        Message message;

        std::map<std::string, bool> members = channel->get_members();
        for (std::map<std::string, bool>::const_iterator it = members.begin(); it != members.end(); ++it) {
            message.add_recipient(it->first);
        }
        message.set_who_code_whom_command_group_message(nickname, "" , "", commands[0],
                                                  commands[1], "");
        server->send_message(socket, message);
    }
    {
        Message message;
        message.set_who_code_whom_command_group_message("ircserv", "353" , nickname, "=",
                                                        commands[1], channel->type_members());
        server->send_message(socket, message);
    }
    {
        Message message;
        message.set_who_code_whom_command_group_message("ircserv", "366" , nickname, "",
                                                        commands[1], "End of /NAMES list.");
        server->send_message(socket, message);
    }

	return COM_NORMAL;
}

int Connection::func_part() {

	if (!check_authorized())
		return COM_NORMAL;

	return COM_NORMAL;
}

void Connection::parse_command_buff() {

	std::stringstream first_part(command_buff.substr(0, command_buff.find(':')));
	std::string parsed;

	while (getline(first_part, parsed, ' ')) {
		commands.push_back(parsed);
	}

	if (command_buff.find(':') != std::string::npos) {
		commands.push_back(command_buff.substr( command_buff.find(':') + 1));
	}
}


