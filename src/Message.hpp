//
// Created by Александр Шоршин on 12.07.2022.
//

#ifndef IRC_CHAT_MESSAGE_HPP
#define IRC_CHAT_MESSAGE_HPP

#include <iostream>
#include <set>

class Message {
public:
	Message(std::string message);
	void add_recipient(std::string recipient);
	std::set<std::string> get_nicknames();
	std::string get_message();

private:
	std::set<std::string> nicknames;
	std::string message;
};


#endif //IRC_CHAT_MESSAGE_HPP
