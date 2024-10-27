
#pragma once

#include <iostream>
#include <climits>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "../configFile/Directive.hpp"
#include "../configFile/Location.hpp"


class   Server
{
    public:
        std::map<std::string, Directive> directives;
        std::map<std::string, Location> locations;
#include <arpa/inet.h>
#include <map>
#include <unistd.h>

#include "../configFile/Directive.hpp"
#include "../configFile/Location.hpp"
#include "../KQueue/KQueue.hpp"

typedef struct s_sockData {
    sockaddr_in* sockAddress;
    socklen_t* sockLen;
    s_sockData(): sockAddress (NULL), sockLen(NULL) {}
} t_sockData;

class   Server
{

public:
    Server();
    ~Server();
    void init();
    int getSocket() const {return m_socket;}

    sockaddr_in m_sockAddress;
    socklen_t m_sockLen;
    t_sockData m_sockData;
    t_eventData m_sEventData;

    std::map<std::string, Directive> directives;
    std::map<std::string, Location> locations;

private:
    int m_socket;
    void    server();
}