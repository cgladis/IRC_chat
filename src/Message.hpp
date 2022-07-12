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

	void add_recipient(const std::string &recipient);
	std::set<std::string> get_nicknames() const;
	bool nickname_in_recipient_list(const std::string &nickname) const;
	std::string get_message() const;
	bool is_self_only() const;

private:
	std::set<std::string> nicknames;
	std::string message;
};

std::ostream &operator<<(std::ostream &out, const Message &message);

#endif //IRC_CHAT_MESSAGE_HPP
