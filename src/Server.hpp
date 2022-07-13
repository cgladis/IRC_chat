//
// Created by Commando Gladis on 7/5/22.
//

#ifndef IRC_CHAT_SERVER_HPP
#define IRC_CHAT_SERVER_HPP

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>
#include <map>
#include "server_commands.h"
#include "Connection.hpp"
#include "colors.hpp"
#include "Database.hpp"
#include "Message.hpp"

#define COMMAND_BUFFER_SIZE 100

class Connection;

class Server {

public:
    Server(int port, std::string password);
    ~Server();
	int getPort() const;

	void init();
	void run();
	void send_message(int fd, const Message &answer) const;
	bool check_password(const std::string &pass) const;

private:
	Server(const Server &){}
	Server &operator=(const Server &){return *this;}


	int fd_socket;
	int port;
	bool exit;
	std::string password;
	std::map<int, Connection*> connection;

	Database database;

};

std::ostream &operator<<(std::ostream &, const Server &srv);


#endif
