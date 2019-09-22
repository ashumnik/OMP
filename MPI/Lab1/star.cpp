#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include "star.hpp"

void star(std::string message_to_root, std::string message_to_edges){
    
    char recieved[256] = {0}; 

    int world_size;
    int rank;
    MPI_Init(nullptr, nullptr);
    
    // Получаем количество процессов и их текущий ранг
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0){
 //       std::cout << "world size is: " << world_size << std::endl;
    }

    int next_proc = rank+1;
    
    for(int round = M; round > 0; round--){
//        std::cout << std::setfill('-') << std::setw(40) << "Current round is " << round << " in [" << rank << "]" << std::endl;

        if(rank == 0){
            for(int side_proc = 1; side_proc < world_size; side_proc++){
                // Послать сообщение всем крайним процессам
                MPI_Send(message_to_edges.c_str(),
                         message_to_edges.size()+1, //+1 из-за '\0' 
                         MPI_CHAR, 
                         side_proc, 
                         0,
                         MPI_COMM_WORLD); 
            }
            for(int side_proc = 1; side_proc < world_size; side_proc++){
                // Принять сообщения от всех крайних процессов
                MPI_Recv(recieved, 
                         sizeof(recieved), 
                         MPI_CHAR,
                         side_proc,
                         0,
                         MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                //std::cout << "It's root process, and i recieved this:{" << recieved << "} from [" << side_proc << "]" << '\n';
                //std::cout << "Begining of broadcast..." << std::endl;

            }
        }
        else{
            // Принять сообщение от главного процесса
            MPI_Recv(recieved, 
                     sizeof(recieved), 
                     MPI_CHAR,
                     0,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            //std::cout << "It's " << rank << " process, and i recieved this:{" << recieved << "}" << std::endl;

            // Послать сообщение главному процессу
            MPI_Send(message_to_root.c_str(),
                     message_to_root.size()+1, //+1 из-за '\0' 
                     MPI_CHAR, 
                     0, 
                     0,
                     MPI_COMM_WORLD); 
        }

    }

    
    MPI_Finalize();
}
