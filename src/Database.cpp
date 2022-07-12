//
// Created by Александр Шоршин on 10.07.2022.
//

#include "Database.hpp"

Database::Database() {
	nicknames.clear();
	users.clear();
	channels.clear();
}

Database::~Database() {
	for (std::map<std::string, Сhannel*>::const_iterator it = channels.begin(); it != channels.end(); ++it){
		delete it->second;
	}
	for (std::map<std::string, User*>::const_iterator it = users.begin(); it != users.end(); ++it){
		delete it->second;
	}
}

bool Database::add_nickname(const std::string &nickname) {

	if (nicknames.find(nickname) == nicknames.end()) {
		nicknames.insert(nickname);
		return true;
	}
	else
		return false;
}

void Database::delete_nickname(const std::string &nickname) {
	if (nicknames.find(nickname) != nicknames.end())
		nicknames.erase(nickname);
}

User* Database::add_user(const std::string &user, const std::string &hostname,
						const std::string &servername, const std::string &realname) {

	if (users.find(user) == users.end()) {
		users[user] = new User(hostname, servername, realname);
		users[user]->set_active(true);
		return users[user];
	}
	else {
		if (users[user]->is_active())
			return NULL;
		else {
			users[user]->set_active(true);
			users[user]->set_hostname(hostname);
			users[user]->set_servername(servername);
			users[user]->set_realname(realname);
			return users[user];
		}
	}
}

void Database::add_channel(const std::string &channel) {
	channels[channel] = new Сhannel;
}


