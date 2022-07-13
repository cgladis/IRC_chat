//
// Created by Александр Шоршин on 12.07.2022.
//

#include "Message.hpp"

Message::Message() {
	message.clear();
	nicknames.clear();
}

Message::Message(const std::string &message): message(message) {
	nicknames.clear();
}

Message::Message(const Message &other) {
	*this = other;
}

Message &Message::operator = (const Message &other) {

	nicknames = other.nicknames;
	who = other.who;
	code = other.code;
	whom = other.whom;
	command = other.command;
	group = other.group;
	message = other.message;
	return *this;
}

void Message::add_recipient(const std::string &recipient) {
	nicknames.insert(recipient);
}

std::set<std::string> Message::get_nicknames() const{
	return nicknames;
}

bool Message::nickname_in_recipient_list(const std::string &nickname) const{
	return nicknames.find(nickname) != nicknames.end();
}

std::string Message::get_message() const {

	std::string result;
	if (!who.empty())
		result += ':' + who + ' ';
	if (!code.empty())
		result += code + ' ';
	if (!whom.empty())
		result += whom + ' ';
	if (!command.empty())
		result += command + ' ';
	if (!group.empty())
		result += group + ' ';
	if (!message.empty())
		result += ':' + message;
	if (!result.empty())
		result += '\n';
	return result;
}

bool Message::is_self_only() const {
	return nicknames.empty();
}

void Message::set_who_code_whom_command_message(std::string who, std::string code, std::string whom,
												std::string command, std::string message) {
	this->who = who;
	this->code = code;
	this->whom = whom;
	this->command = command;
	this->message = message;


}

std::ostream &operator<<(std::ostream &out, const Message &message){
	out << message.get_message();
	return out;
}






