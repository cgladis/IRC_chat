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
