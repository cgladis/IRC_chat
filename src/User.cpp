//
// Created by Александр Шоршин on 12.07.2022.
//

#include "User.hpp"

User::User(std::string hostname, std::string servername, std::string realname):
	hostname(hostname),
	servername(servername),
	realname(realname),
	active(false){

}

void User::set_active(bool flag) {
	active = flag;
}

bool User::is_active() {
	return active;
}

void User::set_hostname(const std::string &hostname) {
	this->hostname = hostname;
}

void User::set_servername(const std::string &servername) {
	this->servername = servername;
}

void User::set_realname(const std::string &realname) {
	this->realname = realname;
}
