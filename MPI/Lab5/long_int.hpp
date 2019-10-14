#ifndef HPP_LONG_INT
#define HPP_LONG_INT
#include "mpi.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <string>
#include <bitset>
#include <vector>
#include <tuple>
#include <cstddef>
#define BIT_AT(X) (this->container[X])

typedef char recursion_stop_type ;

char recv_buffer_root [100000];

template<std::size_t SIZE_IN_BYTES> class LongInt{

    std::bitset<SIZE_IN_BYTES*8> container;

public:
    LongInt();
    LongInt(int seed);
    LongInt(recursion_stop_type number);
    LongInt(std::string serialized);
    LongInt(const LongInt& ln); 

    LongInt<SIZE_IN_BYTES>& operator=(const LongInt<SIZE_IN_BYTES>& other);

    bool operator[](int i);


    LongInt<SIZE_IN_BYTES*2> operator*(const LongInt<SIZE_IN_BYTES>& other);
    LongInt<SIZE_IN_BYTES> operator+(const LongInt<SIZE_IN_BYTES>& other);
    LongInt<SIZE_IN_BYTES> operator-(const LongInt<SIZE_IN_BYTES>& other);
    LongInt<SIZE_IN_BYTES> operator<<(std::size_t count);

    std::size_t size(){
        return this->container.size();
    }

    std::bitset<SIZE_IN_BYTES*8>& get_container(){
        return this->container;
    }

    std::tuple<LongInt<SIZE_IN_BYTES/2>,LongInt<SIZE_IN_BYTES/2>> slice();

    void flip(){
        this->container.flip();
    }

    void reset(){
        this->container.reset();
    }

    std::string ToString();
    
};

template<typename std::size_t SIZE_IN_BYTES>
std::tuple<LongInt<SIZE_IN_BYTES/2>,LongInt<SIZE_IN_BYTES/2>> LongInt<SIZE_IN_BYTES>::slice(){

    LongInt<SIZE_IN_BYTES/2> higher_half, lower_half;
    auto long_int_size = this->size();
    
    for(std::size_t i = 0; i < long_int_size; i++){
        if(i < long_int_size/2){
            lower_half.get_container()[i] = this->container[i];
        }
        else{
            higher_half.get_container()[i/long_int_size] = this->container[i];
        }
    }

    return std::make_tuple(higher_half, lower_half);
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(){
    std::srand(1);
    for(std::size_t i = 0; i < this->size(); i++ ){
        BIT_AT(i) = std::rand()%2;
    }
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(int seed){
    std::srand(seed);
    for(std::size_t i = 0; i < this->size(); i++ ){
        BIT_AT(i) = std::rand()%2;
    }
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(recursion_stop_type number){
    this->container = std::bitset<SIZE_IN_BYTES*8> (number);
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(std::string serialized){
    this->container = std::bitset<SIZE_IN_BYTES*8> (serialized);
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(const LongInt& ln){
    this->container = ln.get_container();
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>& LongInt<SIZE_IN_BYTES>::operator=(const LongInt<SIZE_IN_BYTES>& other){
    this->container = other.get_container();
    return *this;
};

template<typename std::size_t SIZE_IN_BYTES>
bool LongInt<SIZE_IN_BYTES>::operator[](int i){
    return this->container[i];
};


template<typename std::size_t SIZE_IN_BYTES>
std::string LongInt<SIZE_IN_BYTES>::ToString(){
    
    std::stringstream out_stream;

    std::string binary_repr = this->container.to_string();

    char one_chunk = 0;
    std::size_t chunk_bit_count = sizeof(one_chunk)*8;

    for(std::size_t i = 0; i < binary_repr.size(); i += chunk_bit_count){
        auto str_chunk = binary_repr.substr(i, chunk_bit_count);
        out_stream << std::hex << std::stoll(str_chunk, nullptr, 2);
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
LongInt<SIZE_IN_BYTES> LongInt<SIZE_IN_BYTES>::operator<<(std::size_t count){
    LongInt<SIZE_IN_BYTES> result(*this);
    result.get_container() <<= count;
    return result;
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES> LongInt<SIZE_IN_BYTES>::operator-(const LongInt<SIZE_IN_BYTES>& other){

    LongInt<SIZE_IN_BYTES> result(*this);
    LongInt<SIZE_IN_BYTES> one;
    LongInt<SIZE_IN_BYTES> inverted = other;

    inverted.flip();

    result = result + inverted + one;

    return result;

};


template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES*2> LongInt<SIZE_IN_BYTES>::operator*(const LongInt<SIZE_IN_BYTES>& other){
    LongInt<SIZE_IN_BYTES*2> result;

    if(SIZE_IN_BYTES == sizeof(recursion_stop_type)){
        recursion_stop_type mult_product = this->container.to_ulong() * other.get_container().to_ulong();
        return (LongInt<SIZE_IN_BYTES*2>((recursion_stop_type)mult_product));
    }
    // http://courses.csail.mit.edu/6.006/spring11/exams/notes3-karatsuba

    auto t_halves = this->slice();
    auto o_halves = other.slice();

    auto x_h = std::get<0>(t_halves);
    auto y_h = std::get<0>(o_halves);
    auto x_l = std::get<1>(t_halves);
    auto y_l = std::get<1>(o_halves);

    int rank, comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if(rank != 0){
        auto a = x_h * y_h;
        auto d = x_l * y_l;
        auto e = (x_h + x_l) * (y_h + y_l) - a - d;

    }

    if(rank == 0){
        auto k_result = (a << SIZE_IN_BYTES*2) + (e << SIZE_IN_BYTES) + d; 
    }

    for(std::size_t i = 0; i < k_result.size(); i++){
        result.get_container()[i] = k_result[i];
    }


    return result;
};
#endif