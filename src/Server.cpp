//
// Created by Commando Gladis on 7/5/22.
//

#include "Server.hpp"


Server::Server(int port, std::string password): port(port), exit(false), password(password) {
}

Server::~Server(){
    for (std::map<int, Connection*>::const_iterator it = connection.begin(); it != connection.end(); ++it) {
        close(it->first);
        delete(it->second);
    }
	close(fd_socket);
    std::cout << "Server stopped..." << std::endl;
}

int Server::getPort() const {
	return port;
}

void Server::init() {
	fd_socket = socket(2, 1, 0);
	if (fd_socket < 0)
		throw std::runtime_error("Can't get fd of socket");

	sockaddr_in socket_adr;
	socket_adr.sin_family = 2;
	socket_adr.sin_addr.s_addr = (1 << 24) | 127;
	socket_adr.sin_port = port << 8 | port >> 8;

	if (bind(fd_socket, (const struct sockaddr *) &socket_adr, sizeof(socket_adr)) < 0)
		throw std::runtime_error("Can't bind socket");
	if (listen(fd_socket, 128) < 0)
		throw std::runtime_error("Can't listen socket");

	std::cout << "Bind server..." << std::endl;
}

void Server::run() {
	fd_set actual, readyRead, readyWrite;
	sockaddr_in addr;
	socklen_t addr_len = sizeof addr;
	int max_socket = fd_socket;
	FD_ZERO(&actual);
	FD_SET(fd_socket, &actual);

	while (!exit) {
		readyRead = readyWrite = actual;
		if (select(max_socket + 1, &readyRead, &readyWrite, NULL, NULL) < 0) {
			continue;
		}

        if (FD_ISSET(fd_socket, &readyRead)) {
            int clientSock = accept(fd_socket, (struct sockaddr *) &addr, &addr_len);
            if (clientSock < 0)
                continue;
            if (clientSock > max_socket)
                max_socket = clientSock;
            FD_SET(clientSock, &actual);
            connection.insert(
                    std::pair<int, Connection *>(clientSock, new Connection(clientSock, this, &database)));
            std::cout << C_GREEN << "CLIENT " << clientSock << " connected" << C_WHITE << std::endl;
        }

		for (int current_fd = 0; current_fd <= max_socket; current_fd++) {

			if (FD_ISSET(current_fd, &readyRead) && current_fd != fd_socket) {
				char tempstr[COMMAND_BUFFER_SIZE];
				ssize_t res = recv(current_fd, tempstr, COMMAND_BUFFER_SIZE, 0);
				if (res > 0) {
					for (int i = 0; i < res; ++i) {
						if (tempstr[i] == '\n') {
							std::cout << C_BLUE << "CLIENT " << current_fd << " << "
									  << connection[current_fd]->get_command_buff() << C_WHITE << std::endl;
							int answer = connection[current_fd]->runCommand();
							if (answer == COM_EXIT) {
								exit = true;
								break;
							}
                            if (answer == COM_QUIT) {
                                FD_CLR(current_fd, &actual);
                                close(current_fd);
                                delete connection[current_fd];
                                connection.erase(current_fd);
                                std::cout << C_RED << "CLIENT " << current_fd << " disconnected" << C_WHITE << std::endl;
                                break;
                            }
						} else if (tempstr[i] == '\r')
							continue;
						else
							connection[current_fd]->addLetterToBuff(tempstr[i]);
					}
				} else {
                    {
                        Message message;

                        std::set<std::string> channels = connection[current_fd]->get_channels();
                        for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
                            add_recipients_from_channel(*it, connection[current_fd]->get_nickname(), message);
                        }
                        message.set_who_code_whom_command_message(connection[current_fd]->get_nickname(), "" , "",
                                                                        "QUIT","Connection closed");
                        send_message(current_fd, message);
                    }
					FD_CLR(current_fd, &actual);
					close(current_fd);
					delete connection[current_fd];
					connection.erase(current_fd);
					std::cout << C_RED << "CLIENT " << current_fd << " disconnected" << C_WHITE << std::endl;
				}
			}
		}
	}
}


void Server::send_message(int fd, const Message &answer) const {
	std::string message = answer.get_message();
	if (answer.is_self_only()) {
		send(fd, message.c_str(), message.length(), 0);
        std::cout << C_MINT << "CLIENT " << fd << " >> "
                  << message << C_WHITE;
	} else{
		for (std::map<int, Connection*>::const_iterator it = connection.begin(); it != connection.end(); ++it){
			if (answer.nickname_in_recipient_list(it->second->get_nickname())) {
                send(it->first, message.c_str(), message.length(), 0);
                std::cout << C_MINT << "CLIENT " << it->first << " >> "
                          << message << C_WHITE;
            }
		}
	}

}

void Server::add_recipients_from_channel(const std::string &channel_name, const std::string &exept, Message &answer) {
    Channel *channel = database.get_channel(channel_name);
    if (channel){
        std::map<std::string, bool> members = channel->get_members();
        for (std::map<std::string, bool>::const_iterator it = members.begin(); it != members.end(); ++it){
            if (it->first != exept)
                answer.add_recipient(it->first);
        }
    } else
        throw std::runtime_error("channel_name is not exist");
}

bool Server::check_password(const std::string &pass) const {
    return pass == password;
}



std::ostream &operator<<(std::ostream &out, const Server &srv){
    out << "Server http://127.0.0.1:" << srv.getPort();
    return out;
}
