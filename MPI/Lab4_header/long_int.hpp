#ifndef HPP_LONG_INT
#define HPP_LONG_INT
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <string>
#include <bitset>
#include <vector>
#include <cstddef>
#define BIT_AT(X) (this->container[X])

template<std::size_t SIZE_IN_BYTES> class LongInt{

    std::bitset<SIZE_IN_BYTES*8> container;

public:
    LongInt();
    LongInt(int seed);
    LongInt(std::string debug);
    LongInt(const LongInt& ln); 

    LongInt<SIZE_IN_BYTES>& operator=(const LongInt<SIZE_IN_BYTES>& other);

    bool operator[](int i);


    LongInt<SIZE_IN_BYTES*2> operator*(const LongInt<SIZE_IN_BYTES>& other);
    LongInt<SIZE_IN_BYTES> operator+(const LongInt<SIZE_IN_BYTES>& other);

    std::size_t size(){
        return this->container.size();
    }

    std::bitset<SIZE_IN_BYTES*8>& get_container(){
        return this->container;
    }


    void reset();

    std::string ToString();
    
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(){
    std::srand(1);
    for(std::size_t i = 0; i < this->container.size(); i++ ){
        BIT_AT(i) = std::rand()%2;
    }
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(int seed){
    std::srand(seed);
    for(std::size_t i = 0; i < this->container.size(); i++ ){
        BIT_AT(i) = std::rand()%2;
    }
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(std::string debug){
    this->container.set();
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(const LongInt& ln){
    this->container = ln.get_container();
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>& LongInt<SIZE_IN_BYTES>::operator=(const LongInt<SIZE_IN_BYTES>& other){
    this->container(other.container);
    return this;
};

template<typename std::size_t SIZE_IN_BYTES>
bool LongInt<SIZE_IN_BYTES>::operator[](int i){
    return this->container[i];
};

template<typename std::size_t SIZE_IN_BYTES>
void LongInt<SIZE_IN_BYTES>::reset(){
    this->container.reset();
};

template<typename std::size_t SIZE_IN_BYTES>
std::string LongInt<SIZE_IN_BYTES>::ToString(){
    
    std::stringstream out_stream;
    std::size_t proxy_int = 0;
    decltype(proxy_int) bit_count = SIZE_IN_BYTES*8;

    std::size_t index = 0;
    for(;index < this->size(); index++){
        proxy_int ^= (static_cast<decltype(proxy_int)>
                        (this->container[index]) << index % bit_count);

        
        if(index % (bit_count-1) == 0 && index != 0){
            out_stream << std::hex << proxy_int;
            proxy_int = 0;
        }
    }

    return out_stream.str();
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES> LongInt<SIZE_IN_BYTES>::operator+(const LongInt<SIZE_IN_BYTES>& other){

    LongInt<SIZE_IN_BYTES> result(*this);

    std::size_t index = 0;
    std::size_t int_size = this->get_container().size();
    bool carry = false;
    for(;index < int_size; index++){

        //should carry
        if(result[index] & other[index]){
            std::size_t carry_index = index;

            while(result[carry_index] && carry_index < int_size -1){
                result.get_container().flip(carry_index);
                carry_index++;
            }
            result.get_container().flip(carry_index);
            continue;
        }

        result.get_container()[index] = result[index] ^ other[index];
    }
    return result;

};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES*2> LongInt<SIZE_IN_BYTES>::operator*(const LongInt<SIZE_IN_BYTES>& other){
    LongInt<SIZE_IN_BYTES*2> result;
    result.reset();
    std::vector<LongInt<SIZE_IN_BYTES*2>> intermediate_ints;


    /*ANDing*/
    for(auto tbit : this->container){
        LongInt<SIZE_IN_BYTES*2> intermediate;
        intermediate.reset();
        std::size_t index = 0;
        for(auto obit : other.container){
            intermediate[index++] = tbit & obit;
        }
        intermediate_ints.pushback(intermediate);
    }


    /*SUMming*/
    for(auto _int : intermediate_ints){
        result = result + _int;
    }

    return result;
};
#endif
