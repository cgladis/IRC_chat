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

Message &Message::operator=(const Message &other) {
	message = other.message;
	nicknames = other.nicknames;
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
	return message;
}

bool Message::is_self_only() const {
	return nicknames.empty();
}

std::ostream &operator<<(std::ostream &out, const Message &message){
	out << message.get_message();
	return out;
}






