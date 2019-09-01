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

bool ParseLines(std::vector<std::string>* lines);
std::vector<std::string>* LoadFile(std::string filename);
void PrintMachine(Node* root);


struct Node{
    int state;
    bool is_final = false;
    std::map<char, int> next_states;
    std::map<int, Node*> states_to_nodes;
    
    Node* GetNextState(char ch){
        // TODO: mb change [] to at()
        return states_to_nodes[next_states[ch]];
    }

    void PrintNode(char ch){
        std::cout << std::setw(10) << "q" << state;
        std::cout << std::setw(10) << "," << ch;
        auto next_node = GetNextState();
        std::cout << std::setw(10) << "=" << next_node->is_final ? 'f' : 'q' ;
        std::cout << std::setw(10) <<        next_node->state;
        std::cout << std::endl;
    }

    void PrintHierarhy(){
        std::cout << std::setw(10) << std::setfill("-") << std::endl;

        for(auto ref_state : next_states){
            std::cout << std::setw(10) << "q" << state;
            std::cout << std::setw(10) << "," << std::get<0>(ref_state);
            auto next_node = states_to_nodes[std::get<0>(ref_state)];
            std::cout << std::setw(10) << "=" << next_node->is_final ? 'f' : 'q' ;
            std::cout << std::setw(10) <<        next_node->state;
            std::cout << std::endl;
        }

        std::cout << std::setw(10) << std::setfill("-") << std::endl;
    }

} Root;
std::map<int, Node*> all_states;


void main (){

    
}

void Analyze(std::string str_to_analyze){
    Node* current_node = &Root;

    for(auto ch : str_to_analyze){
        if(current_node->next_states.find(ch)){
            current_node->PrintNode(ch);
            current_node = current_node->GetNextState(ch);
        }
    }
}

std::vector<std::string>* LoadFile(std::string filename){
    std::ifstream file_reader(filename, std::ifstream::in);
    std::vector lines;
    std::string line;
    while(std::getline(file_reader, line)){
        lines.push_back(line);
    }

    return new vector<std::string>(lines);
}

void ParseLines(std::vector<std::string>* lines){
    std::regex re("^q(\\d+),(.)=(q|f)(\\d+)$");
    std::smatch match;

    for(auto line : *lines){
        std::regex_search(line, match, re);
        auto state = std::atoi(match[1]);
        auto ch = match[2].c_str()[0];
        auto type_of_next_state = match[3].c_str()[0];
        auto next_state = std::atoi(match[4]);
        
        Node* current_node = new Node;

        current_node.state = state;
        
        current_node.next_states[ch] = next_state;
        if(type_of_next_state == 'f'){
            current_node.is_final = true;
        }
        if(initial_state == 0){
            Root = current_node;
        }

        all_states[state] = current_node;
    }

    for(auto state : all_states){
        Node* current_node = std::get<1>(state);
        auto state_number = std::get<0>(state);
        current_node.states_to_nodes[state_number] = all_states[state_number];
    }
}
