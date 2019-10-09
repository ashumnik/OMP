
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
              << l_int3.ToString()
              << std::endl;

    auto l_d_int1 = LongInt<1>(1);
    auto l_d_int2 = LongInt<1>(1);
    auto l_d_int3 = l_d_int1 + l_d_int2;

    std::cout << l_d_int1.ToString()
              << " + " 
              << l_d_int2.ToString()
              << " = " 
              << l_d_int3.ToString()
              << std::endl;

    std::cout << l_d_int1.get_container().to_string() << std::endl;
    std::cout << l_d_int1.get_container().to_string() << std::endl;
    std::cout << l_d_int3.get_container().to_string() << std::endl;

    return 0;
}

