#ifndef __my_msh_h__
#define __my_msh_h__

#include <msh.h>
#include <cstdint>

namespace Schwarm
{
    constexpr char SIMU_SERVER_ADDR[] = "127.0.0.1";
    constexpr uint16_t SIMU_SERVER_PORT = 10000;

    constexpr uint16_t DETECTION_SERVER_PORT = 10001;

    constexpr uint16_t CONTROL_SERVER_PORT = 10002;
}

#endif // __my_msh_h