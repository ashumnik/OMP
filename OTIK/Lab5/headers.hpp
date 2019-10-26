#ifndef HEADERS_HPP
#define HEADERS_HPP
#include <cstddef>
#include <string>
#include <stdexcept>

struct ArchiveHeader{
    std::size_t magic = 0xBA0BAB;
    std::size_t header_size;
    std::size_t offset_to_next_header;
    FileHeader file_header;
    std::string filename;

    static bool VerifyHeader(std::uint8_t* data){
        ArchiveHeader header(data);
        if(header.magic == 0xBA0BAB){
            return true;
        }
        else{
            return false;
        }
    }

    ArchiveHeader(std::uint8_t* data){
        if(this->VerifyHeader(data)){
            throw std::invalid_argument("Not a valid header");
        }
        
        ArchiveHeader* ptr = reinterpret_cast<ArchiveHeader*>(data);
        
        this->header_size           = ptr->header_size;
        this->offset_to_next_header = ptr->offset_to_next_header;
        this->file_header           = ptr->file_header;
        this->filename              = std::string(data + offsetof(ArchiveHeader,filename));
    }

    std::uint8_t* get_compressed_data(std::uint8_t* data){
        return data + header_size;
    }

};

struct FileHeader{
    std::size_t byte_freq[256];
};

#endif
