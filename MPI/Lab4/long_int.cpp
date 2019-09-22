#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include "long_int.hpp"

#define D(X) (std::cout << X << std::endl)
#define SEND_RECIEVE_BYTES (5)

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(){
    std::srand(1);
    for(auto bit : this->container){
        bit = std::rand()%2;
    }
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(int seed){
    std::srand(seed);
    for(auto bit : this->container){
        bit = std::rand()%2;
    }
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES>::LongInt(const LongInt& ln){
    this->container(ln);
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
    decltype(proxy_int) bit_count = sizeof(proxy_int)*8;

    decltype(std::bitset::count()) index = 0;
    for(;index < this->size(); index++){
        proxy_int ^= (static_cast<decltype(proxy_int)>
                        (this->container[index]) << index % bit_count);
        
        if(index % bit_count == 0 && index != 0){
            out_stream << std::hex << std::to_string() << proxy_int;
            proxy_int = 0;
        }
    }

    return out_stream.str();
};

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES> LongInt<SIZE_IN_BYTES>::operator+(const LongInt<SIZE_IN_BYTES>& other){

    LongInt<SIZE_IN_BYTES*2>container.size()> result;
    result.reset();
    decltype(std::bitset::count()) index = 0;
    bool carry = false;
    for(;index < result.size(); index++){
        if(carry){
            result.flip(index);
            if(!(result[index] & other[index])){
                carry = false;
            }
            continue;
        }
        if(result[index] & other[index]){
            carry = true;
            result.flip(index);
            continue;
        }
    }
}

template<typename std::size_t SIZE_IN_BYTES>
LongInt<SIZE_IN_BYTES*2> LongInt<SIZE_IN_BYTES>::operator*(const LongInt<SIZE_IN_BYTES>& other){
    LongInt<SIZE_IN_BYTES*2>container.size()> result;
    result.reset();
    std::vector<LongInt<SIZE_IN_BYTES*2>> intermediate_ints;


    /*ANDing*/
    for(auto tbit : this->container){
        LongInt<SIZE_IN_BYTES*2> intermediate;
        intermediate.reset();
        decltype(std::bitset::count()) index = 0;
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
/*
void star(std::string message_to_root, std::string message_to_edges){
    
    char recieved_by_edges[256] = {0}; 
    char recieved_by_root[256] = {0}; 
    char sended_to_edges[256] = {0}; 
    char sended_to_root[256] = {0}; 

    int world_size;
    int rank;
    MPI_Init(nullptr, nullptr);
    
    // Получаем количество процессов и их текущий ранг
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0){
        std::cout << "world size is: " << world_size << std::endl;
    }

    int next_proc = rank+1;
    
    for(int round = M; round > 0; round--){
        std::cout << std::setfill('-') << std::setw(40) << "Current round is " << round << " in [" << rank << "]" << std::endl;

        if(round == M){
            std::strcpy(sended_to_edges, message_to_edges.c_str());
            D(sended_to_edges);
        }

        // Послать сообщение всем крайним процессам
        MPI_Scatter(
                  sended_to_edges,
                  SEND_RECIEVE_BYTES, 
                  MPI_CHAR, 
                  recieved_by_edges,
                  SEND_RECIEVE_BYTES,
                  MPI_CHAR,
                  0, 
                  MPI_COMM_WORLD); 
        
        // Принять сообщения от всех крайних процессов

        if(rank == 0){ 
            std::cout << "It's root process, and i recieved this:{" << recieved_by_root << "}" << '\n';
            std::cout << "Begining of scatter..." << std::endl;
        }
        else{
            // Принять сообщение от главного процесса
            std::cout << "It's " << rank << " process, and i recieved this:{" << recieved_by_edges << "}" << std::endl;
        }

        if(round == M){
            std::strcpy(sended_to_root, message_to_root.c_str());
            D(sended_to_root);
        }

        // Послать сообщение главному процессу
        MPI_Gather(
                 sended_to_root,
                 SEND_RECIEVE_BYTES, 
                 MPI_CHAR, 
                 recieved_by_root,
                 SEND_RECIEVE_BYTES,
                 MPI_CHAR, 
                 0, 
                 MPI_COMM_WORLD); 

    }

    
    MPI_Finalize();
}
*/
