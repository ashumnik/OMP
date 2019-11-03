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
#include <cstddef>
#define BIT_AT(X) (this->container[X])

char recv_buffer_root [100000];

template<std::size_t SIZE_IN_BYTES> class LongInt{

    std::bitset<SIZE_IN_BYTES*8> container;

public:
    LongInt();
    LongInt(int seed);
    LongInt(std::string serialized);
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
LongInt<SIZE_IN_BYTES*2> LongInt<SIZE_IN_BYTES>::operator*(const LongInt<SIZE_IN_BYTES>& other){
    LongInt<SIZE_IN_BYTES*2> result;
    result.reset();
    std::vector<LongInt<SIZE_IN_BYTES*2>> intermediate_ints;


    /*ANDing*/
    std::size_t int_size = this->get_container().size();
    
    for(std::size_t i = 0;i < int_size; i++){
        auto tbit = this->container[i];

        LongInt<SIZE_IN_BYTES*2> intermediate;
        intermediate.reset();
        for(std::size_t j = 0 ;j < int_size; j++){
            auto obit = other.get_container()[j];
            intermediate.get_container()[j] = tbit & obit;
        }
        intermediate.get_container() <<= i;

        intermediate_ints.push_back(intermediate);
    }

    std::size_t inter_ints_byte_size = intermediate_ints.size()*sizeof(result);


    int rank, comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    std::size_t size_to_send = inter_ints_byte_size/comm_size;
    std::size_t count_to_process = intermediate_ints.size()/comm_size;

    /*
    MPI_Scatter(
            reinterpret_cast<void*>(&intermediate_ints[0]),
            size_to_send,
            MPI_BYTE,
            reinterpret_cast<void*>(recv_buffer_other),
            size_to_send,
            MPI_BYTE,
            0,
            MPI_COMM_WORLD
            );
            */

    for(std::size_t i = 0; i < count_to_process; i++){
        //std::cout << intermediate_ints[rank*count_to_process + i].ToString() << std::endl;
        result = result + intermediate_ints[rank*count_to_process + i];
    }

    MPI_Barrier(MPI_COMM_WORLD);

    auto bitstring = result.get_container().to_string();
    auto bitstring_size = result.get_container().to_string().size();

    MPI_Gather(
            reinterpret_cast<void*>(const_cast<char*>(bitstring.c_str())),
            bitstring_size + 1,
            MPI_CHAR,
            reinterpret_cast<void*>(recv_buffer_root),
            bitstring_size + 1,
            MPI_CHAR,
            0,
            MPI_COMM_WORLD
            );

    if(rank == 0){

        std::size_t serialized_size = 0;

        for(std::size_t i = 0; i < comm_size-1; i++){
            serialized_size += std::strlen(recv_buffer_root)+1;
            LongInt<SIZE_IN_BYTES*2> deserialized(&recv_buffer_root[serialized_size]);

            result = result + deserialized;
        }
    }

    return result;
};
#endif
