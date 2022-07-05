//
// Created by Commando Gladis on 7/5/22.
//

#ifndef IRC_CHAT_SERVER_HPP
#define IRC_CHAT_SERVER_HPP

#include <iostream>

class server {

public:
    server();

    server(const server &);

    ~server();

    server &operator=(const server &);

};

std::ostream &operator<<(std::ostream &, const server &);


#endif
