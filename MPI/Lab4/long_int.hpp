#ifndef HPP_LONG_INT
#define HPP_LONG_INT
#include <string>
#include <bitset>
#include <vector>
#include <cstddef>

template<std::size_t SIZE_IN_BYTES> class LongInt{

    std::bitset<SIZE_IN_BYTES*8> container;

public:
    LongInt();
    LongInt(int seed);
    LongInt(const LongInt& ln); 

    LongInt<SIZE_IN_BYTES>& operator=(const LongInt<SIZE_IN_BYTES>& other);

    bool operator[](int i);


    LongInt<SIZE_IN_BYTES*2> operator*(const LongInt<SIZE_IN_BYTES>& other);
    LongInt<SIZE_IN_BYTES> operator+(const LongInt<SIZE_IN_BYTES>& other);

    decltype(std::bitset::size()) size(){
        return container.size();
    }

    void reset();

    std::string ToString();
    
};

#endif
