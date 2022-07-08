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


class Server {

public:
    Server(int port);
    ~Server();
	int getPort() const;
	void init();
	void run();

private:
	Server(const Server &){}
	Server &operator=(const Server &){return *this;}


	int fd_socket;
	int port;
	bool exit;

};

std::ostream &operator<<(std::ostream &, const Server &srv);


#endif
