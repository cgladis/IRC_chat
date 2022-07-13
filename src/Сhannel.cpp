//
// Created by Александр Шоршин on 12.07.2022.
//

#include "Сhannel.hpp"

std::string Channel::get_members() const {
	std::string result;

	for (std::map<std::string, bool>::const_iterator it = members.begin(); it != members.end(); ++it) {
		if (it->second)
			result += "@" + it->first;
		else
			result += it->first;
		result += ' ';
	}
	return std::string();
}
