#ifndef HPP_LONG_INT
#define HPP_LONG_INT
#include <string>
#include <bitset>
#include <cstddef>

template<std::size_t SIZE_IN_BYTES> class LongInt{

    std::bitset<SIZE_IN_BYTES*8> container;

public:
    LongInt();
    LongInt(int seed);
    LongInt(const LongInt& ln); 
    LongInt& operator=(LongInt other);
    LongInt operator*(LongInt other);
};

#endif
