//
// Created by Commando Gladis on 7/5/22.
//

#include "Client.hpp"

Client::~Client() {

	close(fd_socket);
	std::cout << name << " stopped..." << std::endl;
}

void Client::init() {
	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_socket < 0)
		throw std::runtime_error("Can't get fd of socket");

	sockaddr_in socket_adr;
	socket_adr.sin_family = AF_INET;
	socket_adr.sin_addr.s_addr = inet_addr(addr_ip.data());
	socket_adr.sin_port = htons(port);

	if (connect(fd_socket, (const struct sockaddr *) &socket_adr, sizeof socket_adr) < 0)
		throw std::runtime_error("Can't connect to server");

	std::cout << "Connection success..." << std::endl;

	{
		Message message;
		message.command_whom_message("PASS", password, "");
		send_message(message);
	}
	{
		Message message;
		message.command_whom_message("NICK", name, "");
		send_message(message);
	}
	{
		Message message;
		message.command_whom_message("USER", name + " 0 *", name);
		send_message(message);
	}
	for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
		{
			Message message;
			message.command_whom_message("JOIN", *it, "");
			send_message(message);
		}
	}
}

void Client::run() {

	while (!exit) {
		char tempstr[COMMAND_BUFFER_SIZE];
		ssize_t res = recv(fd_socket, tempstr, COMMAND_BUFFER_SIZE, 0);
		if (res > 0) {
			for (int i = 0; i < res; ++i) {
				if (tempstr[i] == '\n') {
					std::cout << C_BLUE << name << " << "
							  << command_buff << C_WHITE << std::endl;
					runCommand();
				} else if (tempstr[i] == '\r')
					continue;
				else
					command_buff.push_back(tempstr[i]);
			}
		} else {
			exit = true;
			close(fd_socket);
			std::cout << C_RED << name << " disconnected" << C_WHITE << std::endl;
		}
	}
}


void Client::send_message(const Message &answer) const {

	std::string message = answer.get_message();
	send(fd_socket, message.c_str(), message.length(), 0);
	std::cout << C_MINT << name << " >> "
			  << message << C_WHITE;
}

std::string Client::get_name() const {
	return name;
}

std::string Client::get_data_from_api() {

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("159.65.220.83");
	addr.sin_port = htons(80);

	if (connect(sock, (const struct sockaddr *) &addr, sizeof addr) < 0)
		return "Service unavailable";

	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);

	std::string header = "GET /" + std::to_string(tm.tm_mon + 1) + "/"
						 + std::to_string(tm.tm_mday) + "/date HTTP/1.1\n";
	header += "Host: 159.65.220.83\n";
	header += "Accept: */*\n";
	header += "User-Agent: " + name + "\n";
	header += "Accept-Encoding: gzip, deflate\n";
	header += "Accept-Language: en-US,en;q=0.9,ru;q=0.8\n";
	header += "Connection: keep-alive\n\n";

	send(sock, header.data(), header.length(), 0);

	char buf[1024];
	size_t rez = recv(sock, buf, sizeof(buf), 0);
	buf[rez] = '\0';

	close(sock);

	std::string str = buf;

	return str.substr(str.find("\r\n\r\n") + 4);
}

void Client::runCommand() {
	parse_command_buff();

	if (commands.size() > 3){
		if (commands[1] == "JOIN"){
			{
				Message message;
				message.command_whom_message("PRIVMSG", commands[2],
											 "Hi, " + commands[0] + "!");
				send_message(message);
			}
		}
	}

	commands.clear();
	command_buff.clear();
}

void Client::parse_command_buff() {


	std::stringstream first_part(command_buff.substr( command_buff.find(':')));
	std::string parsed;

	while (getline(first_part, parsed, ' ')) {
		commands.push_back(parsed);
	}

	if (command_buff.find(':', 1) != std::string::npos) {
		commands.push_back(command_buff.substr( command_buff.find(':', 1) + 1));
	}

}


std::ostream &operator<<(std::ostream &out, const Client &bot) {
	out << bot.get_name();
	return out;
}
