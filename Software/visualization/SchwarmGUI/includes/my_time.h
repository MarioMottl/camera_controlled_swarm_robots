#ifndef __my_time_h__
#define __my_time_h__

#include <ctime>
#include <sstream>

/*
*   Static is used there becuase it is only defined and used in the translation unit of the
*   "my_msg.h" include file.
*   Since only one header or .cpp file uses this header, there will only be one copy of this function.
*   Static is not necessary if only one header implements that function.
*   Just in case there is another file that includes this include file.
*/

static void get_time(int delay, int& s, int& m, int& h)
{
    // get time
    __time64_t t;
    _time64(&t);

    // decode time
    s = t % 60;
    m = (t / 60) % 60;
    h = ((t / 3600) + delay) % 24;
}

static std::string time_to_string(int s, int m, int h)
{
    std::stringstream ss;
    ss << ((h < 10) ? "0" : "") << h << ":" << ((m < 10) ? "0" : "") << m << ":" << ((s < 10) ? "0" : "") << s;
    return ss.str();
}

#endif // __my_time_h__
