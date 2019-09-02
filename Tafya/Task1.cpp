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
    
    static bool doesExist(int state);

    Node* GetNextState(char ch){
        // TODO: mb change [] to at()
        return states_to_nodes.at(next_states.at(ch));
    }

    void PrintNode(char ch){
        auto next_node = GetNextState(ch);
        std::cout << std::setw(6) << std::setfill(' ') << "q" << state;
        std::cout << "," << ch;
        std::cout << "=" << (next_node->is_final ? 'f' : 'q');
        std::cout <<        next_node->state;
        std::cout << std::endl;
    }

    void PrintHierarhy(){
        for(auto ref_state : next_states){
            auto next_node = GetNextState(std::get<0>(ref_state));

            std::cout << std::setw(6) << "q" << state;
            std::cout << "," << std::get<0>(ref_state);
            std::cout << "=" << (next_node->is_final ? 'f' : 'q');
            std::cout <<        next_node->state;
            std::cout << std::endl;
        }
    }

} Root;

std::map<int, Node*> all_states;

bool Node::doesExist(int state){
    return all_states.count(state) == 1; 
}

std::vector<std::string>* GetLines(std::istream* input);
std::vector<std::string>* LoadFile(std::string filename);
void ParseLines(std::vector<std::string>* lines);
void Analyze(std::string str_to_analyze);

void PrintAll(){
    for(auto state : all_states){
        std::get<1>(state)->PrintHierarhy();
    }
}

int main (){
    auto lines = LoadFile("test.txt");
    ParseLines(lines);

    Root.PrintNode('a');
    std::cout << std::endl;

    // PrintAll();
    Analyze("ba +c");
}

void Analyze(std::string str_to_analyze){
    Node* current_node = &Root;

    for(auto ch : str_to_analyze){

        if(current_node->next_states.count(ch) >= 1){
            std::cout << "Correct [" << ch << "] : "; 
            current_node->PrintNode(ch);
            current_node = current_node->GetNextState(ch);
        }
        else{
            std::cout << "Incorrect [" << ch << "]" << std::endl;
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

void ParseLines(std::vector<std::string>* lines){
    std::regex re("^q(\\d+),(.)=(q|f)(\\d+)$");
    std::smatch match;
    
    for(auto line : *lines){
        std::regex_search(line, match, re);
        auto state_number = std::atoi(match[1].str().c_str());
        auto ch = match[2].str().c_str()[0];
        auto type_of_next_state = match[3].str().c_str()[0];
        auto next_state = std::atoi(match[4].str().c_str());
        
        Node* current_node;
        if(Node::doesExist(state_number)){
            current_node = all_states[state_number];
        }
        else{
            current_node = new Node;
            current_node->state = state_number;
        }

        current_node->next_states[ch] = next_state;

        if(type_of_next_state == 'f'){
            current_node->is_final = true;
        }

        all_states[state_number] = current_node;
    }


    for(auto state : all_states){
        auto current_node = std::get<1>(state);
        auto state_number = std::get<0>(state);

        for(auto next_state : current_node->next_states){
            auto next_state_number = std::get<1>(next_state);
            current_node->states_to_nodes[next_state_number] = all_states[next_state_number];
        }
        if(state_number == 0){
            Root = *current_node;
        }

    }
}
