//
// Created by Александр Шоршин on 10.07.2022.
//

#ifndef IRC_CHAT_DATABASE_HPP
#define IRC_CHAT_DATABASE_HPP

#include <set>
#include <map>
#include <iostream>
#include "User.hpp"
#include "Сhannel.hpp"

class Database {
public:
	Database();
	~Database();
	bool add_nickname(const std::string &nickname);
	void delete_nickname(const std::string &nickname);

	User* add_user(const std::string &user, const std::string &hostname,
				  const std::string &servername, const std::string &realname);

	Channel* add_channel(const std::string &channel);
    bool is_channel_exist(const std::string &channel);

private:
	std::set<std::string> nicknames;
	std::map<std::string, User*> users;
	std::map<std::string, Channel*> channels;
};


#endif //IRC_CHAT_DATABASE_HPP
