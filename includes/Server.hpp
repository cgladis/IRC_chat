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
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <map>
#include "server_commands.h"
#include "Connection.hpp"
#include "colors.hpp"
#include "Database.hpp"
#include "Message.hpp"
#include <arpa/inet.h>

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
    void add_recipients_from_channel(const std::string &channel_name, const std::string &exept, Message &answer);
	bool check_password(const std::string &pass) const;

	std::string get_name() const;

	std::string get_data_from_api();

    void set_nickname_to_kill(const std::string &nickname);

    Connection *get_connection(const std::string &nickname);

private:
	Server(const Server &){}
	Server &operator=(const Server &){return *this;}


	int fd_socket;
	int port;
	bool exit;
	std::string password;
	std::map<int, Connection*> connection;

	Database database;

	std::string name;

    std::string nickname_to_kill;

};

std::ostream &operator<<(std::ostream &, const Server &srv);


#endif
