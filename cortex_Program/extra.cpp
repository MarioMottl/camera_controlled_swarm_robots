#include "extra.hpp"

void ul_to_string(char *dst, size_t dstsize, unsigned long input)
{
    char *start = dst;
    size_t len = 0;
    do
    {
        uint8_t digit;
        digit = input % 10;
        *dst++ = digit + '0';
        dstsize--;
        len++;
        input /= 10;
    } while ((input != 0) && (dstsize > 0));
    *dst++ = '\0';
    memrev(start, len-1);
}

void memrev(void *_beg, size_t len)
{
    char *start = (char*)_beg,
         *end = start + len;
    while (start < end)
    {
        char a = *start;
        *start = *end;
        *end = a;
        ++start;
        --end;
    }
}
