//
// Created by Александр Шоршин on 12.07.2022.
//

#include "Message.hpp"

Message::Message(std::string message): message(message) {
	nicknames.clear();
}

void Message::add_recipient(std::string recipient) {
	nicknames.insert(recipient);
}

std::set<std::string> Message::get_nicknames() {
	return nicknames;
}

std::string Message::get_message() {
	return message;
}
