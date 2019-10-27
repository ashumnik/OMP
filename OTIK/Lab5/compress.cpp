#include "compress.hpp"

#define FOR_RANGE(X) for(std::size_t _ = 0; _ < sizeof(X); _++)
#define SAVE_IN_BYTES(WHAT, WHERE) { \
            std::uint8_t* WHAT ## ptr = reinterpret_cast<std::uint8_t*>(&WHAT);\
            FOR_RANGE(sizeof(WHAT)){ WHERE->push_back(*WHAT ## ptr++); }}

std::size_t SummFreq(FileHeader* file_header){
    std::size_t sum = 0;
    for(auto one_byte_freq : file_header->byte_freq){
        sum += one_byte_freq;
    }
    return sum;
}

FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data){
    FREQ_MAP_TYPE* fh = new FREQ_MAP_TYPE; 
    memcpy(fh->byte_freq, ArchiveHeader(data).file_header.byte_freq, sizeof(FileHeader::byte_freq));
    return fh;
}

FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data, std::size_t data_size){

    FREQ_MAP_TYPE* fh = new FREQ_MAP_TYPE; 
    std::memset(fh->byte_freq, 0, sizeof(fh->byte_freq));

    for(std::size_t i = 0; i < data_size; i++){
        fh->byte_freq[data[i]]++;
    }
    return fh;
}


std::vector<std::uint8_t>* Decompress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies){
    std::vector<std::uint8_t>* decompressed_data = new std::vector<std::uint8_t>;

    return decompressed_data;
}

std::vector<std::uint8_t>* Compress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies){
    std::vector<std::uint8_t>* compressed_data = new std::vector<std::uint8_t>;
    std::size_t block_size     = 1024;
    std::size_t remaining_size = data_size;
    std::size_t number_repr_msg;

    for(std::size_t i = 0; remaining_size > 0; i++){
        if(remaining_size - block_size < 0){
            SAVE_IN_BYTES(number_repr_msg, compressed_data);
        }
        if(remaining_size % block_size == 0){
            
        }
    }
    return compressed_data;
}
