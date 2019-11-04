#include "compress.hpp"
#include <tuple>
#include <iostream>
#include <iomanip>

#define RANGE 30
#define AROUND(X,Y) X - RANGE < Y && X + RANGE > Y
#define ABS(X) X < 0 ? -X : X
#define FOR_RANGE(X) for(std::size_t _ = 0; _ < sizeof(X); _++)
#define SAVE_IN_BYTES(WHAT, WHERE) { \
            std::uint8_t* WHAT ## ptr = reinterpret_cast<std::uint8_t*>(&WHAT);\
            FOR_RANGE(sizeof(WHAT)){ WHERE->push_back(*WHAT ## ptr++); }}


std::size_t SummFreq(FileHeader* file_header, std::uint8_t until){
    std::size_t sum = 0;
    for(int i = 0; i < UINT8_MAX; i++ ){
        sum += file_header->byte_freq[i];
        if(i == until){
            return sum;
        }
    }
    return sum;
}

std::size_t SummFreq(FileHeader* file_header){
    std::size_t sum = 0;
    for(auto one_byte_freq : file_header->byte_freq){
        sum += one_byte_freq;
    }
    return sum;
}

//std::tuple<std::size_t, std::size_t, std::uint8_t> 
std::uint8_t get_range4decode(
        std::size_t encoded_n,
        FREQ_MAP_TYPE* file_header,
        std::vector<std::uint8_t>* container,
        std::size_t* l, std::size_t* h){

    for(std::uint8_t b = 1; b < 256; b++){
        std::size_t r = *h - *l + 1;                         // интервал
        *h = *l + (r*SummFreq(file_header, b))/BLOCK_SIZE;   // новая верхняя граница
        *l = *l + (r*SummFreq(file_header, b-1))/BLOCK_SIZE; // новая нижняя граница
        std::size_t number_repr_block = (*l+*h)/2;
        container->push_back(b);

        if(b < 10)
            std::cout << (int)b << " b- " << std::hex << number_repr_block  << std::endl;
        if(AROUND(encoded_n, number_repr_block)){
            //return std::make_tuple(l, h, b);
            return b;
        }
    }
    throw new std::out_of_range("Encoded number don't fall into regions");
}

FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data){
    FREQ_MAP_TYPE* fh = new FREQ_MAP_TYPE; 
    auto a_header = ArchiveHeader(data);
    memcpy(fh->byte_freq, a_header.file_header.byte_freq, sizeof(FileHeader::byte_freq));
    fh->original_size = a_header.file_header.original_size;
    return fh;
}

FREQ_MAP_TYPE* CountFrequencies(std::uint8_t* data, std::size_t data_size){

    FREQ_MAP_TYPE* fh = new FREQ_MAP_TYPE; 
    fh->original_size = data_size;
    std::memset(fh->byte_freq, 0, sizeof(fh->byte_freq));

    for(std::size_t i = 0; i < data_size; i++){
        fh->byte_freq[data[i]]++;
    }
    return fh;
}


std::vector<std::uint8_t>* Decompress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* file_header){
    std::vector<std::uint8_t>* decompressed_data = new std::vector<std::uint8_t>;

    std::size_t remaining_size     = data_size;
    std::size_t number_repr_block;
    std::size_t size_to_decompress = data_size/sizeof(number_repr_block);
    std::size_t l                  = 0;
    std::size_t h                  = TOP;

        number_repr_block = *reinterpret_cast<decltype(number_repr_block)*>(data);
    for(std::size_t i = 0; remaining_size > 0; i++, remaining_size--){

        //auto r_tuple = get_range4decode(number_repr_block, file_header);
        //auto r = std::get<1>(r_tuple) - std::get<0>(r_tuple);
        //std::size_t l_diff = number_repr_block - std::get<1>(r_tuple);
        //auto byte = std::get<2>(r_tuple);
        auto byte = get_range4decode(number_repr_block, file_header, decompressed_data, &l, &h);
        decompressed_data->push_back(byte);

        //l = l + r*SummFreq(file_header, byte-1)/BLOCK_SIZE;
        //h = l + r*SummFreq(file_header, byte)/BLOCK_SIZE;

        if(i == file_header->original_size - 1){
            break;
        }
        break;

            /*
        if(remaining_size % BLOCK_SIZE == 0 && i != 0){
            SAVE_IN_BYTES(number_repr_block, decompressed_data);
            l = 0;
            h = TOP;
        }
        if(BLOCK_SIZE > data_size && i == data_size){
            SAVE_IN_BYTES(number_repr_block, decompressed_data);
        }
        */
    }

    return decompressed_data;
}

std::vector<std::uint8_t>* Compress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* file_header){
    std::vector<std::uint8_t>* compressed_data = new std::vector<std::uint8_t>;
    std::size_t remaining_size     = data_size-1;
    std::size_t number_repr_block;
    std::size_t l                  = 0;
    std::size_t h                  = TOP;

    for(std::size_t i = 1; remaining_size > 0; i++, remaining_size--){
        std::size_t r = h - l + 1;                               // интервал
        h = l + (r*SummFreq(file_header, data[i]))/BLOCK_SIZE;   // новая верхняя граница
        l = l + (r*SummFreq(file_header, data[i]-1))/BLOCK_SIZE; // новая нижняя граница
        number_repr_block = (l+h)/2;

        std::cout << i << " -- " << number_repr_block << std::endl;
        if(remaining_size % BLOCK_SIZE == 0 && i != 0){
            SAVE_IN_BYTES(number_repr_block, compressed_data);
            l = 0;
            h = TOP;
        }
    }
    if(remaining_size == 0 && compressed_data->size() == 0){
        SAVE_IN_BYTES(number_repr_block, compressed_data);
    }
    return compressed_data;
}
