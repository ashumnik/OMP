#include "compress.hpp"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]){

    char example[] = {0,1,2,3,4};
    std::cout << example << std::endl;

    for(auto b : example){
        std::cout << " " << std::hex << (int)b ;
    }
    std::cout << std::endl;

    auto compressed_data = *Compress(reinterpret_cast<std::uint8_t*>(example), sizeof(example), CountFrequencies(reinterpret_cast<std::uint8_t*>(example), sizeof(example)));
    for(auto b : compressed_data){
        std::cout << " " << std::hex << (int)b ;
    }
    std::cout << std::endl;


    auto decompressed_data = *Decompress(&compressed_data[0], compressed_data.size(), CountFrequencies(reinterpret_cast<std::uint8_t*>(example), sizeof(example)));
    for(auto b : decompressed_data){
        std::cout << " " << std::hex << (int)b; 
    }
    std::cout << std::endl;
    std::cout << example << std::endl;

    /*
    for(int i = 2; i < argc; i++){
        auto filename = argv[i];
        auto file = open(filename, O_RDONLY, 0);

        struct stat st;
        stat(filename, &st);

        auto mapped_file = static_cast<uint8_t*>(mmap(NULL,st.st_size, PROT_READ, MAP_POPULATE ,file,0));


        switch(argv[1][0])
        {
            case 'd':
                      auto frequencies = CountFrequencies(mapped_file);
                      Decompress(mapped_file, st.st_size, frequencies);
                      break;
            case 'c': 
                      auto frequencies = CountFrequencies(mapped_file, st.st_size);
            default : Compress(mapped_file, st.st_size, frequencies);
        }
    }

*/
    return 0;
}

