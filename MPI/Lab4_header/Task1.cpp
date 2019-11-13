
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstddef>

#include "long_int.hpp"

int main(int argc, char *argv[]){

    MPI_Init(nullptr, nullptr);
    
    int rank, comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    const std::size_t size = 100;
    LongInt<size> l_int1(1);
    LongInt<size> l_int2(2);
    auto l_int3 = l_int1 * l_int2;
    
    if(rank == 0)
    std::cout << l_int1.ToString() 
              << " * " 
              << l_int2.ToString()
              << " = " 
              << l_int3.ToString()
              << std::endl;

    MPI_Finalize();

    return 0;
}

