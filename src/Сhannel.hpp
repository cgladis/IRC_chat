//
// Created by Александр Шоршин on 12.07.2022.
//

#ifndef IRC_CHAT_СHANNEL_HPP
#define IRC_CHAT_СHANNEL_HPP

#include <iostream>
#include <map>

class Channel {

public:
    Channel(const std::string &name);

	std::string type_members() const;

    std::map<std::string, bool> get_members() const;
    std::string get_name() const;

    void add_member(const std::string &member);
    void del_member(const std::string &member) ;
    void set_operator(const std::string &member, bool is_operator);

    bool    oper_check(std::string nick);
    bool    member_check(std::string nick);

    size_t count_members();

private:

	std::map<std::string, bool> members; // nickname + operator
    std::string name;

};


#endif //IRC_CHAT_СHANNEL_HPP
