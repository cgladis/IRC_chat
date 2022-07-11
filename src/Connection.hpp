//
// Created by Александр Шоршин on 10.07.2022.
//

#ifndef IRC_CHAT_CONNECTION_HPP
#define IRC_CHAT_CONNECTION_HPP

#include <iostream>
#include <map>
#include "Server.hpp"
#include "Database.hpp"

class Server;

class Connection {

public:

	Connection(Server *server, Database *database): authorized(false), server(server), database(database) {
		comlist["EXIT"] = &Connection::func_exit;
		comlist["PASS"] = &Connection::func_pass;
		comlist["NICK"] = &Connection::func_nick;
	}

	std::string runCommand();
	void addLetterToBuff(char letter);
	std::string get_command_buff() const;
	std::string get_command();

	std::string func_exit();
	std::string func_pass();
	std::string func_nick();

private:
	bool authorized;
	std::string command_buff;

	std::map<std::string, std::string (Connection::*)()> comlist;

	Server *server;
	Database *database;

	std::string nickname;

};


#endif //IRC_CHAT_CONNECTION_HPP
