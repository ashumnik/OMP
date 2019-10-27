#include "compress.hpp"
#include <tuple>

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

std::tuple<std::size_t, std::size_t, std::uint8_t> get_range4decode(std::size_t encoded_n, FREQ_MAP_TYPE* frequencies){
    std::size_t l = 0;
    std::size_t h = TOP;

    for(std::size_t b = 1; b < 256; b++){
        std::size_t r = h - l + 1;                                          // интервал
        l = l + (r*SummFreq(frequencies, b-1))/BLOCK_SIZE; // новая нижняя граница
        h = l + (r*SummFreq(frequencies, b))/BLOCK_SIZE;  // новая верхняя граница
        std::size_t number_repr_block = (l+h)/2;
        if(AROUND(encoded_n, number_repr_block)){
            return std::make_tuple(l, h, b);
        }
    }
    throw new std::out_of_range("Encoded number don't fall into regions");
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

    std::size_t remaining_size     = data_size;
    std::size_t number_repr_block;
    std::size_t size_to_decompress = data_size/sizeof(number_repr_block);
    std::size_t l                  = 0;
    std::size_t h                  = TOP;

    for(std::size_t i = 0; remaining_size > 0; i++, remaining_size--){
        number_repr_block = *reinterpret_cast<decltype(number_repr_block)*>(data);

        auto r_tuple = get_range4decode(number_repr_block, frequencies);
        auto r = std::get<1>(r_tuple) - std::get<0>(r_tuple);
        std::size_t l_diff = number_repr_block - std::get<1>(r_tuple);
        decompressed_data->push_back(std::get<2>(r_tuple));

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

std::vector<std::uint8_t>* Compress(std::uint8_t* data, std::size_t data_size, FREQ_MAP_TYPE* frequencies){
    std::vector<std::uint8_t>* compressed_data = new std::vector<std::uint8_t>;
    std::size_t remaining_size     = data_size-1;
    std::size_t number_repr_block;
    std::size_t l                  = 0;
    std::size_t h                  = TOP;

    for(std::size_t i = 1; remaining_size > 0; i++, remaining_size--){
        std::size_t r = h - l + 1;                                          // интервал
        l = l + (r*SummFreq(frequencies, data[i]-1))/BLOCK_SIZE; // новая нижняя граница
        h = l + (r*SummFreq(frequencies, data[i]))/BLOCK_SIZE;  // новая верхняя граница
        number_repr_block = (l+h)/2;

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
