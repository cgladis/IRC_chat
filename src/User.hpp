//
// Created by Александр Шоршин on 12.07.2022.
//

#ifndef IRC_CHAT_USER_HPP
#define IRC_CHAT_USER_HPP

#include <iostream>

class User {
public:
	User();
	void set_active(bool flag);
	bool is_active();
	void set_hostname(const std::string &hostname);
	void set_servername(const std::string &servername);
	void set_realname(const std::string &realname);
	void set_operator(bool is_operator);
	void set_password(const std::string &password);


private:
	std::string hostname;
	std::string servername;
	std::string realname;
	bool active;

	bool is_operator;
	std::string password;
};


#endif //IRC_CHAT_USER_HPP
