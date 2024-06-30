#ifndef MANAGEBITS_H
#define MANAGEBITS_H

#include <cstdint>

// Sets the nth bit of a uint64_t to 1 or 0 if n between 0 and 63, included
void set_nth_bit(uint64_t& bits, uint8_t n, bool bitValue);

// Get the nth bit of a uint64_t 1 or 0 if n between 0 and 63, included
bool get_nth_bit(uint64_t bits, uint8_t n);

#endif
