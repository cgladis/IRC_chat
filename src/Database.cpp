//
// Created by Александр Шоршин on 10.07.2022.
//

#include "Database.hpp"

Database::Database() {
	nicknames.clear();
}

bool Database::add_nickname(const std::string &nickname) {
	if (nicknames.size() == 0){
		nicknames.insert(nickname);
		return true;
	}
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
