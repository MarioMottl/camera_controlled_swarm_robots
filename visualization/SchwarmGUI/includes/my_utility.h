#ifndef __my_utility_h__
#define __my_utility_h__

#include <string>
#include <cppsock.hpp>

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

    inline bool simu_connected(cppsock::tcp::socket* client)
    {
        const std::string& serveraddr = client->sock().getpeername().get_addr();
        uint16_t port = client->sock().getpeername().get_port();

        return (serveraddr == Schwarm::PATH_SERVER_ADDR && port == Schwarm::PATH_SERVER_PORT);
    }
};

#endif //__my_utility_h__