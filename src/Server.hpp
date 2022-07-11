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
#include "errors.hpp"
#include "Connection.hpp"

#define COMMAND_BUFFER_SIZE 100

class Server {

public:
    Server(int port);
    ~Server();
	int getPort() const;
	void init();
	void run();
	void send_message_to_socket(int fd, const std::string &message) const;

private:
	Server(const Server &){}
	Server &operator=(const Server &){return *this;}


	int fd_socket;
	int port;
	bool exit;
	std::map<int, Connection*> connection;


};

std::ostream &operator<<(std::ostream &, const Server &srv);


#endif
