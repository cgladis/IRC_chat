//
// Created by Александр Шоршин on 12.07.2022.
//

#include "Сhannel.hpp"

Channel::Channel(const std::string &name): name(name) {
    members.clear();
}

std::string Channel::type_members() const {
	std::string result;

	for (std::map<std::string, bool>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (it->second)
			result += "@" + it->first;
		else
			result += it->first;
		result += ' ';
	}
	return result;
}

size_t Channel::count_members() {
    return members.size();
}

std::map<std::string, bool> Channel::get_members() const {
    return members;
}


std::string Channel::get_name() const {
    return name;
}


void Channel::add_member(const std::string &member) {

    members.insert(std::pair<std::string, bool>(member, false));
    if (members.size() == 1)
        set_operator(member, true);
}

void Channel::del_member(const std::string &member) {

    if (members.find(member) != members.end())
        members.erase(member);
}

void Channel::set_operator(const std::string &member, bool is_operator) {

    if (members.find(member) != members.end())
        members.find(member)->second = is_operator;
}

bool    Channel::oper_check(std::string nick)
{
    return  members[nick];
}

bool Channel::member_check(std::string nick)
{
    std::map<std::string, bool>::iterator   it = members.find(nick);
    if (it != members.end())
        return true;

    return false;
}