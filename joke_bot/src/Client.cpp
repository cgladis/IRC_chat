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
					commands.clear();
					command_buff.clear();
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


void Client::runCommand() {
	parse_command_buff();

	if (commands.size() >= 3){
		if (commands[0] == name)
			return;

		if (commands[1] == "JOIN") {
			std::vector<std::string> messages;
			messages.push_back("Hi, " + commands[0] + "!");
			messages.push_back("I know all about numbers");
			messages.push_back("Type any number");

			for (std::vector<std::string>::const_iterator it = messages.begin(); it != messages.end(); ++it) {
				{
					Message message;
					message.command_whom_message("PRIVMSG", commands[2],
												 *it);
					send_message(message);
				}
			}
		}
		if (commands[1] == "PRIVMSG" and commands[2] != name){
			{
				Message message;
				message.command_whom_message("PRIVMSG", commands[2],
											 get_info_about_numbers(atoi(commands[3].data())));
				send_message(message);
			}
		}
		else if (commands[1] == "PRIVMSG" and commands[2] == name){
			{
				Message message;
				message.command_whom_message("PRIVMSG", commands[0],
											 get_info_about_numbers(atoi(commands[3].data())));
				send_message(message);
			}
		}
	}
}

void Client::parse_command_buff() {

	if (command_buff[0] == ':')
		command_buff.erase(0,1);
	std::stringstream first_part(command_buff.substr(0,command_buff.find(':')));
	std::string parsed;

	while (getline(first_part, parsed, ' ')) {
		commands.push_back(parsed);
	}

	if (command_buff.find(':') != std::string::npos) {
		commands.push_back(command_buff.substr( command_buff.find(':') + 1));
	}

}

std::string Client::get_info_about_numbers(int nomber) {

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("159.65.220.83");
	addr.sin_port = htons(80);

	if (connect(sock, (const struct sockaddr *) &addr, sizeof addr) < 0)
		return "Service unavailable";

	std::string header = "GET /" + std::to_string(nomber) + " HTTP/1.1\n";
	header += "Host: 159.65.220.83\n";
	header += "Accept: */*\n";
	header += "User-Agent: " + name + "\n";
	header += "Accept-Encoding: gzip, deflate\n";
	header += "Accept-Language: en-US,en;q=0.9,ru;q=0.8\n";
	header += "Connection: keep-alive\n\n";

	send(sock, header.data(), header.length(), 0);

	char buf[2048];
	size_t rez = recv(sock, buf, sizeof(buf), 0);
	buf[rez] = '\0';

	close(sock);

	std::string str = buf;
	return str.substr(str.find("\r\n\r\n") + 4);
}

std::ostream &operator<<(std::ostream &out, const Client &bot) {
	out << bot.get_name();
	return out;
}
