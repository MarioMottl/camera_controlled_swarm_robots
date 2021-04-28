#ifndef __my_msg_h__
#define __my_msg_h__

#include "my_time.h"

/*
*   Inline tells the compiler to have only one definition of the function in multiple translation units.
*   As this is a shared include file, there will only be one definition of these functions.
*
*   Static can also be used but with the difference that the compiler would 
*   make a copy of these functions for every translation unit. In the resulting executable will
*   be more than one definition.
*
*   With static you would end up with different addresses for the same function in different translation units.
*   With inline you woild end up with only one address for each translation unit.
*   The same counts for global inline variables vs static variables.
*/

inline std::string get_msg(const std::string& prefix)
{
    int s, m, h;
    get_time(2, s, m, h);
    return "[" + time_to_string(s, m, h) + "] " + "[" + prefix + "]: ";
}

#endif // __my_msg_h__
