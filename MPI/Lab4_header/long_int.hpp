#ifndef HPP_LONG_INT
#define HPP_LONG_INT
#include "mpi.h"
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
    this->container = other.get_container();
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
    std::size_t int_size = this->get_container().size();

    for(std::size_t i = 0, index = 0 ;i < int_size; i++){
        auto tbit = this->container[i];
        LongInt<SIZE_IN_BYTES*2> intermediate;
        intermediate.reset();
        for(std::size_t j ;j < int_size; j++){
            auto obit = this->container[j];
            intermediate.get_container()[i++] = tbit & obit;
        }
        intermediate_ints.push_back(intermediate);
        std::cout << std::endl;
        std::cout << intermediate.get_container().to_string() << "|" << std::endl;
    }


    LongInt<SIZE_IN_BYTES*2> recv_buffer [200];

    std::size_t inter_ints_byte_size = intermediate_ints.size()*sizeof(result);
    //std::cout << result.get_container().size() << std::endl;


    int rank, comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    MPI_Scatter(
            reinterpret_cast<void*>(&intermediate_ints[0]),
            inter_ints_byte_size/comm_size,
            MPI_CHAR,
            reinterpret_cast<void*>(recv_buffer),
            inter_ints_byte_size/comm_size,
            MPI_CHAR,
            0,
            MPI_COMM_WORLD
            );

    if(rank != 0){
        for(std::size_t i = 0; i < intermediate_ints.size()/comm_size; i++){
            result = result + recv_buffer[i];
        }
    }

    MPI_Gather(
            reinterpret_cast<void*>(&result),
            sizeof(result),
            MPI_CHAR,
            reinterpret_cast<void*>(recv_buffer),
            sizeof(result),
            MPI_CHAR,
            0,
            MPI_COMM_WORLD
            );

    if(rank == 0){
        for(std::size_t i = 0; i < comm_size; i++){
            result = result + recv_buffer[i];
        }
    }

    return result;
};
#endif
