#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <string>
#include <map>
#include <cstddef>
#include <cstdio>
#include <vector>
#include <cstring>
#include <utility>
#include <type_traits>

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "headers.hpp"

#define FREQ_MAP_TYPE FileHeader
#define TOP (UINT64_MAX / 2 - 1024)
#define BLOCK_SIZE 16

//std::tuple<std::size_t, std::size_t, std::uint8_t> 
std::uint8_t get_range4decode(
        std::size_t encoded_n,
        FREQ_MAP_TYPE* file_header,
        std::vector<std::uint8_t>* container,
        std::size_t* l, std::size_t* h);
std::size_t SummFreq(FileHeader* file_header);
FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data);
FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data, std::size_t data_size);
std::vector<std::uint8_t>* Decompress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies);
std::vector<std::uint8_t>* Compress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies);


#endif
