//
// Created by Александр Шоршин on 10.07.2022.
//

#ifndef IRC_CHAT_DATABASE_HPP
#define IRC_CHAT_DATABASE_HPP

#include <set>
#include <map>
#include <iostream>
#include "User.hpp"

class Database {
public:
	Database();
	bool add_nickname(const std::string &nickname);
	void delete_nickname(const std::string &nickname);
private:
	std::set<std::string> nicknames;
	std::map<std::string, User> users;

};


#endif //IRC_CHAT_DATABASE_HPP
