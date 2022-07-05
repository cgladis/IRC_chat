//
// Created by Commando Gladis on 7/5/22.
//

#include "Server.hpp"

Server::Server(){
    std::cout << "Starting server..." << std::endl;
}

Server::Server(const Server &other) {
    *this = other;
}

Server::~Server(){
    std::cout << "Server stopped..." << std::endl;
}

Server &Server::operator=(const Server &){
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Server &){
    out << "Server";
    return out;
}