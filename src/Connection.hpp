//
// Created by Александр Шоршин on 10.07.2022.
//

#ifndef IRC_CHAT_CONNECTION_HPP
#define IRC_CHAT_CONNECTION_HPP

#include <iostream>
#include <map>


class Connection {

public:
	Connection(): authorized(false){
//		comlist["EXIT"] = std::bind1st(&Connection::func_exit, &this);
	}
	std::string runCommand();
	void addLetterToBuff(char letter);
	std::string get_command_buff() const;

	std::string func_exit(){return "";}

private:
	bool authorized;
	std::string command_buff;
	std::map<std::string, void(*)> comlist;
};


#endif //IRC_CHAT_CONNECTION_HPP
