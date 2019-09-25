
#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstddef>

#include "long_int.hpp"

int main(int argc, char *argv[]){
    const std::size_t size = 8;
    LongInt<size> l_int1(1);
    LongInt<size> l_int2(2);
    auto l_int3 = l_int1 + l_int2;
    
    std::cout << l_int1.ToString() 
              << " + " 
              << l_int2.ToString()
              << " = " 
              << l_int3.ToString();
    return 0;
}

