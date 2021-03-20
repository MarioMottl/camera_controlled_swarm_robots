#ifndef EXTRA_HPP_INCLUDED
#define EXTRA_HPP_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void ul_to_string(char *dst, size_t dstsize, unsigned long input);
void memrev(void *start, size_t len);

#endif // EXTRA_HPP_INCLUDED
