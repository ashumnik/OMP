
#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstddef>
#define N 10
#define M 5

void ring(std::string message);

int main(int argc, char *argv[]){
    std::string message("Message to the world");

    ring(message);

    return 0;
}

void ring(std::string message){
    
    char recieved[256] = {0}; 

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
        if(next_proc < world_size){
            // Послать сообщение следующему процесса
            MPI_Send(message.c_str(),
                     message.size()+1, //+1 из-за '\0' 
                     MPI_CHAR, 
                     next_proc, 
                     0,
                     MPI_COMM_WORLD); 
        }
        else{
            // Послать сообщение первому процессу
            MPI_Send(message.c_str(),
                     message.size()+1, //+1 из-за '\0' 
                     MPI_CHAR, 
                     0, 
                     0,
                     MPI_COMM_WORLD); 
        }
        if(rank == 0){
            // Принять сообщение от последнего процесса
            MPI_Recv(recieved, 
                     sizeof(recieved), 
                     MPI_CHAR,
                     world_size-1,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            std::cout << std::setfill('-') << std::setw(40) << "Current round is " << round << std::endl;
        }
        else{
            // Принять сообщение от предыдущего процесса
            MPI_Recv(recieved, 
                     sizeof(recieved), 
                     MPI_CHAR,
                     rank-1,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }

        std::cout << "It's " << rank << " process, and i recieved this:{" << recieved << "}" << std::endl;
    }



    
    MPI_Finalize();
}
