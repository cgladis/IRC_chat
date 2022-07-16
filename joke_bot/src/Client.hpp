//
// Created by Commando Gladis on 7/5/22.
//

#ifndef IRC_CHAT_SERVER_HPP
#define IRC_CHAT_SERVER_HPP

#include <iostream>
#include <sstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <arpa/inet.h>
#include <vector>
#include "server_commands.h"
#include "colors.hpp"
#include "Message.hpp"

#define COMMAND_BUFFER_SIZE 100

class Client {

public:
    Client(std::string addr_ip, int port, std::string password): addr_ip(addr_ip), port(port), exit(false), password(password) {
		this->name = "joke_bot";

		command_buff.clear();
		commands.clear();


		channels.clear();
		channels.insert("#jokes");
	}
    ~Client();

	void init();
	void run();
	void send_message(const Message &answer) const;

	std::string get_name() const;

	void runCommand();
	void parse_command_buff();

	std::string get_data_from_api();

private:
	Client(const Client &){}
	Client &operator=(const Client &){return *this;}

	int fd_socket;
	std::string addr_ip;
	int port;
	bool exit;
	std::string password;
	std::string name;

	std::string command_buff;
	std::vector<std::string> commands;

	std::set<std::string> channels;

};

std::ostream &operator<<(std::ostream &, const Client &srv);


#endif
