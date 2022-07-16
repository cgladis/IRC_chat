//
// Created by Александр Шоршин on 12.07.2022.
//

#ifndef IRC_CHAT_MESSAGE_HPP
#define IRC_CHAT_MESSAGE_HPP

#include <iostream>
#include <set>

class Message {
public:

	Message();
	Message(const std::string &message);
	Message(const Message &other);

	Message &operator = (const Message &other);

	std::string get_message() const;
    void command_whom_message(const std::string &command, const std::string &whom, const std::string &message);

private:
	std::string command;
	std::string whom;
	std::string message;
};

std::ostream &operator<<(std::ostream &out, const Message &message);

#endif //IRC_CHAT_MESSAGE_HPP
