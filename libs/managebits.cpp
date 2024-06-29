#include "managebits.h"

uint64_t set_nth_bit(uint64_t& bits, uint8_t n, bool bitValue)
{
    if (n >= 64)
    {
        //std::cerr << "Bit out of range set" << std::endl;
        return bits;
    }
    if (bitValue)
    {
        bits |= (uint64_t(1) << n);
    }
    else
    {
        bits &= ~(uint64_t(1) << n);
    }
}

bool get_nth_bit(uint64_t bits, uint8_t n) {
    if (n >= 64)
    {
        //std::cerr << "Bit out of range get" << std::endl;
        return false;
    }
    return (bits & (uint64_t(1) << n)) != 0;
}