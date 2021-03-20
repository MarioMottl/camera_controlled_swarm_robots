#ifndef __my_utility_h__
#define __my_utility_h__

#include <string>
#include "../client/my_msh.h"

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

    inline bool simu_connected(SH::Client* client)
    {
        const std::string& serveraddr = client->get_socket().getpeername().get_addr();
        uint16_t port = client->get_socket().getpeername().get_port();

        return (serveraddr == Schwarm::SIMU_SERVER_ADDR && port == Schwarm::SIMU_SERVER_PORT);
    }
};

#endif //__my_utility_h__