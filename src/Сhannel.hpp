//
// Created by Александр Шоршин on 12.07.2022.
//

#ifndef IRC_CHAT_СHANNEL_HPP
#define IRC_CHAT_СHANNEL_HPP

#include <iostream>
#include <map>

class Channel {

public:
	std::string get_members() const;

private:

	std::map<std::string, bool> members; // nickname + operator

};


#endif //IRC_CHAT_СHANNEL_HPP
