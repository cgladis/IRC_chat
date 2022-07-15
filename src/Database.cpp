//
// Created by Александр Шоршин on 10.07.2022.
//

#include "Database.hpp"

Database::Database() {
	nicknames.clear();
	users.clear();
	channels.clear();

    User *user = add_user("admin", "*", "*", "admin");
    user->set_password("admin");
    user->set_operator(true);

}

Database::~Database() {
	for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it){
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

	std::map<std::string, Channel*>::iterator it = channels.begin();
	while (it != channels.end()){
		it->second->del_member(nickname);
		if (it->second->count_members() == 0)
			channels.erase(it);
		it++;
	}
}

User* Database::add_user(const std::string &user, const std::string &hostname,
						const std::string &servername, const std::string &realname) {

	if (users.find(user) == users.end()) {
		users[user] = new User();
        users[user]->set_hostname(hostname);
        users[user]->set_servername(servername);
        users[user]->set_realname(realname);
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

Channel* Database::add_channel(const std::string &channel) {
    if (channels.find(channel) == channels.end()) {
        channels[channel] = new Channel(channel);
    }
    return channels[channel];
}

Channel* Database::get_channel(const std::string &channel){
	if (channels.find(channel) == channels.end()) {
		channels[channel] = NULL; //nullptr
	}
	return channels[channel];
}

bool Database::is_channel_exist(const std::string &channel) {
    return channels.find(channel) != channels.end();
}

void Database::del_channel(const std::string &channel) {
    if (channels.find(channel) != channels.end()) {
        channels.erase(channel);
    }
}


std::map<std::string, Channel *> Database::get_channels() const {
	return channels;
}
