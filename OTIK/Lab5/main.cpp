#include "compress.hpp"

int main(int argc, char* argv[]){

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


    return 0;
}

