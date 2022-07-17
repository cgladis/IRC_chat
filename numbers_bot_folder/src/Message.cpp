//
// Created by Александр Шоршин on 12.07.2022.
//

#include "Message.hpp"

Message::Message() {
}

Message::Message(const Message &other) {
	*this = other;
}

Message &Message::operator = (const Message &other) {

	command = other.command;
	whom = other.whom;
	message = other.message;
	return *this;
}

std::string Message::get_message() const {

	std::string result;

	if (!command.empty())
		result += command + ' ';
	if (!whom.empty())
		result += whom + ' ';
	if (!message.empty())
		result += ':' + message;
	if (!result.empty())
		result += '\n';
	return result;
}

void Message::command_whom_message(const std::string &command, const std::string &whom, const std::string &message) {
    this->command = command;
	this->whom = whom;
    this->message = message;

}

std::ostream &operator<<(std::ostream &out, const Message &message){
	out << message.get_message();
	return out;
}






