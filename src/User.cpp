//
// Created by Александр Шоршин on 12.07.2022.
//

#include "User.hpp"

User::User():
	active(false),
	operator(false){

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

void User::set_operator(bool is_operator) {
    this->is_operator = is_operator;
}

void User::set_password(const std::string &password) {
    this->password = password;
}
