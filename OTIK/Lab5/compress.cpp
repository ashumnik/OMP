#include "compress.hpp"

std::size_t SummFreq(FileHeader* file_header){
    std::size_t sum = 0;
    for(auto one_byte_freq : file_header->byte_freq){
        sum += one_byte_freq;
    }
    return sum
}

FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data){
    FREQ_MAP_TYPE* fh = new FREQ_MAP_TYPE; 
    memcpy(fh->byte_freq, ArchiveHeader(data).file_header->byte_freq, sizeof(FileHeader::byte_freq))
    return fh;
}

FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data, std::size_t data_size){

    FREQ_MAP_TYPE* fh = new FREQ_MAP_TYPE; 
    std::memset(fh->byte_freq, 0, sizeof(fh->byte_freq));

    for(std::size_t i = 0; i < data_size; i++){
        fh->byte_freq[data[i]]++;
    }
    for(auto freq : fh->byte_freq){
        SUMMED_FREQ += freq;
    }
}


std::vector<std::uint8_t>* Decompress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies){
    std::vector<std::uint8_t>* decompressed_data;

}

std::vector<std::uint8_t>* Compress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies){
    std::vector<std::uint8_t>* compressed_data;
}
