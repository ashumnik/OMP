#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <cstring>
#include "star.hpp"

#define D(X) (std::cout << X << std::endl)
#define SEND_RECIEVE_BYTES (5)

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

/*            MPI_Gather(
                 sended_to_root, 
                 message_to_root.size(),
                 MPI_CHAR,
                 recieved_by_root,
                 message_to_root.size(),
                 MPI_CHAR,
                 0,
                 MPI_COMM_WORLD);
                 */
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
