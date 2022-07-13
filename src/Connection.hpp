//
// Created by Александр Шоршин on 10.07.2022.
//

#ifndef IRC_CHAT_CONNECTION_HPP
#define IRC_CHAT_CONNECTION_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include "Server.hpp"
#include "Database.hpp"
#include "Message.hpp"
#include "User.hpp"
#include "server_commands.h"

class Server;

class Connection {

public:

	Connection(int socket, Server *server, Database *database): socket(socket), authorized(false), server(server),
													database(database), user_ref(NULL) {
		commands.clear();
        channels.clear();

		comlist["EXIT"] = &Connection::func_exit;
		comlist["PASS"] = &Connection::func_pass;
		comlist["NICK"] = &Connection::func_nick;
		comlist["USER"] = &Connection::func_user;
		comlist["PONG"] = &Connection::func_pong;
		comlist["QUIT"] = &Connection::func_quit;
		comlist["PRIVMSG"] = &Connection::func_msg;
		comlist["JOIN"] = &Connection::func_join;
		comlist["PART"] = &Connection::func_part;
		comlist["MODE"] = &Connection::func_mode;
	}

	~Connection(){
		database->delete_nickname(nickname);
		if (user_ref)
			user_ref->set_active(false);
	}

	int runCommand();
	void addLetterToBuff(char letter);
	std::string get_command_buff() const;
	std::string get_nickname() const;
	bool check_authorized() const;
	void send_start_massage() const;
	void parse_command_buff();

	int func_exit();
	int func_pass();
	int func_nick();
	int func_user();
	int func_pong();
	int func_quit();
	int func_msg();
	int func_join();
	int func_part();
	int func_mode();

private:
	int socket;
	bool authorized;
	std::string command_buff;
	std::vector<std::string> commands;
    std::set<std::string> channels;

	std::map<std::string, int (Connection::*)()> comlist;

	Server *server;
	Database *database;

	std::string nickname;
	User *user_ref;

};


#endif //IRC_CHAT_CONNECTION_HPP
