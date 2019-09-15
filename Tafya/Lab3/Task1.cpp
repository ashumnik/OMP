#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include <cstdlib>
#include <ctime>

std::vector<std::string>* GetLines(std::istream* input){

    std::string line;
    auto lines = new std::vector<decltype(line)>;

    while(std::getline(*input, line)){
        lines->push_back(line);
    }

    return lines;
}

std::vector<std::string>* LoadFile(std::string filename){
    std::ifstream file_reader(filename, std::ifstream::in);
    return GetLines(&file_reader);
}

int main (){
    auto lines = LoadFile("test.txt");
}

