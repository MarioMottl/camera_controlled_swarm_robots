#ifndef EXTRA_HPP_INCLUDED
#define EXTRA_HPP_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief converts an unsigned long to a string
 * 
 * @param dst pointer to the string buffer
 * @param dstsize size of the string buffer
 * @param input input number to be converted
 */
void ul_to_string(char *dst, size_t dstsize, unsigned long input);
/**
 * @brief reverses a specified memory block
 * 
 * @param start pointer to the start of the memory
 * @param len length of the memory
 */
void memrev(void *start, size_t len);

#endif // EXTRA_HPP_INCLUDED
