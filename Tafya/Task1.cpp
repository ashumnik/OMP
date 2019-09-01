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

struct Node{
    int state = -1;
    bool is_final = false;
    std::map<char, int> next_states;
    std::map<int, Node*> states_to_nodes;
    
    Node* GetNextState(char ch){
        // TODO: mb change [] to at()
        return states_to_nodes[next_states[ch]];
    }

    void PrintNode(char ch){
        auto next_node = GetNextState(ch);
        if(next_node == nullptr){
            std::cout << "this path do not exist" << std::endl;
            return;
        }

        std::cout << std::setw(10) << "q" << state;
        std::cout << "," << ch;
        std::cout << next_node << std::endl;
        std::cout <<        next_node << std::endl;
        std::cout << "=" << next_node->is_final ? 'f' : 'q' ;
        std::cout <<        next_node->state;
        std::cout << std::endl;
    }

    void PrintHierarhy(){
        std::cout << std::setw(10) << std::setfill('-') << std::endl;

        for(auto ref_state : next_states){
            auto next_node = states_to_nodes[std::get<0>(ref_state)];
            if(next_node == nullptr){
                std::cout << "this path do not exist" << std::endl;
                return;
            }

            std::cout << std::setw(10) << "q" << state;
            std::cout << "," << std::get<0>(ref_state);
            std::cout << "=" << next_node->is_final ? 'f' : 'q' ;
            std::cout <<        next_node->state;
            std::cout << std::endl;
        }

        std::cout << std::setw(10) << std::setfill('-') << std::endl;
    }

} Root;
std::map<int, Node*> all_states;

std::vector<std::string>* GetLines(std::istream* input);
std::vector<std::string>* LoadFile(std::string filename);
void ParseLines(std::vector<std::string>* lines);
void Analyze(std::string str_to_analyze);


int main (){
    auto lines = LoadFile("test.txt");
    ParseLines(lines);
    std::cout << Root.next_states.size() << std::endl;
    Root.PrintNode('a');
    std::cout << Root.state << std::endl;
    Root.PrintHierarhy();
    // Analyze("abaa");
}

void Analyze(std::string str_to_analyze){
    Node* current_node = &Root;

    for(auto ch : str_to_analyze){
        
        if(current_node->next_states.find(ch) == current_node->next_states.end()){
            current_node->PrintNode(ch);
            current_node = current_node->GetNextState(ch);
        }
    }
}

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

// Сделать по-человечески
void ParseLines(std::vector<std::string>* lines){
    std::regex re("^q(\\d+),(.)=(q|f)(\\d+)$");
    std::smatch match;
    
#error Не доделано! Работает неправльно

    for(auto line : *lines){
        std::regex_search(line, match, re);
        auto state = std::atoi(match[1].str().c_str());
        auto ch = match[2].str().c_str()[0];
        auto type_of_next_state = match[3].str().c_str()[0];
        auto next_state = std::atoi(match[4].str().c_str());
        
        Node* current_node = new Node;

        current_node->state = state;
        
        current_node->next_states[ch] = next_state;
        if(type_of_next_state == 'f'){
            current_node->is_final = true;
        }
        if(state == 0){
            if(Root.state != 0){
                Root = *current_node;
            }
            else{
            }
        }

        all_states[state] = current_node;
    }


    for(auto state : all_states){
        Node* current_node = std::get<1>(state);
        auto state_number = std::get<0>(state);
        current_node->states_to_nodes[state_number] = all_states[state_number];
    }
}
