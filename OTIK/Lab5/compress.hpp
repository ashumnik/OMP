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
#define TOP INT64_MAX / 2 - 1024


std::size_t SummFreq(FileHeader* file_header);
FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data);
FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data, std::size_t data_size);
std::vector<std::uint8_t>* Decompress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies);
std::vector<std::uint8_t>* Compress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies);


#endif
