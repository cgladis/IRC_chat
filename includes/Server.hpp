//
// Created by Commando Gladis on 7/5/22.
//

#ifndef IRC_CHAT_SERVER_HPP
#define IRC_CHAT_SERVER_HPP

#include <iostream>

class Server {

public:
    Server();

    Server(const Server &);

    ~Server();

    Server &operator=(const Server &);

};

std::ostream &operator<<(std::ostream &, const Server &);


#endif
