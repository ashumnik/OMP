
#include "mpi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstddef>

#include "ring.hpp"
#include "star.hpp"

int main(int argc, char *argv[]){
    std::string message("Message to root");
    std::string message2edges("Message to edges");

    star(message, message2edges);

    return 0;
}

