//
// Created by Commando Gladis on 7/5/22.
//

#include "Server.hpp"


Server::Server(int port, std::string password): port(port), exit(false), password(password) {
}

Server::~Server(){
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

	while (!exit){
		readyRead = readyWrite = actual;
		if (select(max_socket + 1, &readyRead, &readyWrite, NULL, NULL) < 0) {
			continue;
		}

		for (int current_fd = 0; current_fd <= max_socket; current_fd++) {
			if (FD_ISSET(current_fd, &readyRead) && current_fd == fd_socket) {
				int clientSock = accept(fd_socket, (struct sockaddr *) &addr, &addr_len);
				if (clientSock < 0)
					continue;
				if (clientSock > max_socket)
					max_socket = clientSock;
				FD_SET(clientSock, &actual);
				connection.insert(std::pair<int, Connection*>(clientSock, new Connection(this, &database)));
				std::cout << C_GREEN << "CLIENT " << clientSock << " connected"<< C_WHITE << std::endl;
//				send_message_to_socket(clientSock, "Welcome to IRC\n");
			}

			if (FD_ISSET(current_fd, &readyRead) && current_fd != fd_socket) {
				char tempstr[COMMAND_BUFFER_SIZE];
				ssize_t res = recv(current_fd, tempstr, COMMAND_BUFFER_SIZE, 0);
				if (res > 0) {
					for (int i = 0; i < res; ++i) {
						if (tempstr[i] == '\n') {
							std::cout << C_BLUE << "CLIENT " << current_fd << " << "
								<< connection[current_fd]->get_command_buff() << C_WHITE << std::endl;
							Message answer = connection[current_fd]->runCommand();
							if (answer.get_message() == "EXIT") {
								exit = true;
								break;
							} else if (!answer.get_message().empty()) {
								std::cout << C_YELLOW << "CLIENT " << current_fd << " >> "
										  << answer.get_message() << C_WHITE;
								if (answer.is_self_only())
									send_message_to_socket(current_fd, answer.get_message());
								else
									send_message_to_recipients(answer);
							}
						}
						else if (tempstr[i] == '\r')
							continue;
						else
							connection[current_fd]->addLetterToBuff(tempstr[i]);
					}
				} else{
					FD_CLR(current_fd, &actual);
					close(current_fd);
					delete connection[current_fd];
					connection.erase(current_fd);
					std::cout << C_RED << "CLIENT " << current_fd << " disconnected"<< C_WHITE << std::endl;
				}

			}
		}
	}
}

void Server::send_message_to_socket(int fd, const std::string &message) const {
	send(fd, message.c_str(), message.length(), 0);
}

void Server::send_message_to_recipients(const Message &answer) const {
	for (std::map<int, Connection*>::const_iterator it = connection.begin(); it != connection.end(); ++it){
		if (answer.nickname_in_recipient_list(it->second->get_nickname()))
			send_message_to_socket(it->first, answer.get_message());
	}
}

std::string Server::getPassword() const {
	return password;
}

std::ostream &operator<<(std::ostream &out, const Server &srv){
    out << "Server http://127.0.0.1:" << srv.getPort();
    return out;
}
