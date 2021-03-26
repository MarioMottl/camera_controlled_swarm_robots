#include "extra.hpp"

void ul_to_string(char *dst, size_t dstsize, unsigned long input)
{
    char *start = dst;
    size_t len = 0;
    dstsize--; // decrement destination size to always fit the last '\0' character
    if(input == 0) // if input is 0, a special case is needed because the first loop condition wuld skip otherwise
    {
        *dst++ = '0'; // write 0 into string
        len++; // increment length to avoid big problems in memrev
    }
    while ((input != 0) && (dstsize > 0)) // repeat until all digits are processed (input==0) or the destination is full(dstsize==0)
    {
        uint8_t digit;
        digit = input % 10; // get 10^0-digit
        *dst++ = digit + '0'; // convert number to ASCII character
        dstsize--; // decrement destination size
        len++; // increment length of the string number
        input /= 10; // divide the input to get the next digit as 10^0-digit
    }
    *dst++ = '\0'; // write the last terminating character
    // at this point, the number is fully converted, but in reverse order, e.g 1234 becomes "4321"
    memrev(start, len-1); // reverse number and use length to terminate
}

void memrev(void *_beg, size_t len)
{
    // calculate start and end addresses as 1-byte character
    int8_t *start = (int8_t*)_beg,
         *end = start + len;
    while (start < end) // repeat until start surpassed end to avoid double reversing
    {
        // swap *start and *end
        int8_t a = *start;
        *start = *end;
        *end = a;
        // next 2 characters for swapping
        ++start;
        --end;
    }
}
