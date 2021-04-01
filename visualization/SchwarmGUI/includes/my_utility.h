#ifndef __my_utility_h__
#define __my_utility_h__

#include <string>
#include <cppsock.hpp>

#include <iostream>

namespace Schwarm
{
    inline bool is_number(const std::string& str)
    {
        for(char c : str)
        {
            if(c < '0' || c > '9')
                return false;
        }
        return true;
    }

    inline bool simu_connected(std::shared_ptr<cppsock::tcp::socket> client)
    {
#if 1
        const std::string& serveraddr = client->sock().getpeername().get_addr();
        uint16_t port = client->sock().getpeername().get_port();

        return (serveraddr == Schwarm::PATH_SERVER_ADDR && port == Schwarm::PATH_SERVER_PORT);
#else
        int8_t indicator;
        return (client->recv(&indicator, sizeof(int8_t), cppsock::peek) > 0);
#endif
    }
};

#endif //__my_utility_h__