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

	Connection(int socket, Server *server, Database *database): socket(socket), authorized(false), is_operator(false),
                                                    server(server), database(database), user_ref(NULL) {
		commands.clear();
        channels.clear();

		comlist["EXIT"] = &Connection::func_exit;
		comlist["PASS"] = &Connection::func_pass;
		comlist["NICK"] = &Connection::func_nick;
		comlist["USER"] = &Connection::func_user;
		comlist["PING"] = &Connection::func_ping;
		comlist["PONG"] = &Connection::func_pong;
		comlist["QUIT"] = &Connection::func_quit;
		comlist["PRIVMSG"] = &Connection::func_msg;
		comlist["NOTICE"] = &Connection::func_msg;
		comlist["JOIN"] = &Connection::func_join;
		comlist["PART"] = &Connection::func_part;
		comlist["MODE"] = &Connection::func_mode;
		comlist["LIST"] = &Connection::func_list;
		comlist["KICK"] = &Connection::oper_func_kick;
		comlist["INVITE"] = &Connection::oper_func_invite;
		comlist["KILL"] = &Connection::func_kill;
		comlist["OPER"] = &Connection::func_oper;
        comlist["RESTART"] = &Connection::func_restart;
	}

	~Connection();

	int runCommand();
	void addLetterToBuff(char letter);
	std::string get_command_buff() const;
	std::string get_nickname() const;
	bool check_right_password() const;
	void send_start_massage() const;
	void parse_command_buff();

    void add_channel(const std::string &channel);

    std::set<std::string> get_channels();

	bool check_authorized_user_and_message();

	int func_exit();
    int func_quit();
	int func_pass();
	int func_nick();
	int func_user();
	int func_ping();
	int func_pong();
	int func_msg();
	int func_join();
	int func_part();
	int func_mode();
	int func_list();
	int	func_kill();
	int	func_oper();
    int	func_restart();
	
    int oper_func_kick();
    int oper_func_invite();

private:
	int socket;
	bool authorized;
    bool is_operator;
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
