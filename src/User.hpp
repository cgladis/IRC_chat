//
// Created by Александр Шоршин on 12.07.2022.
//

#ifndef IRC_CHAT_USER_HPP
#define IRC_CHAT_USER_HPP

#include <iostream>

class User {
public:
	User(std::string hostname, std::string servername, std::string realname);
	void set_active(bool flag);
	bool is_active();
private:
	std::string hostname;
	std::string servername;
	std::string realname;
	bool active;
};


#endif //IRC_CHAT_USER_HPP
