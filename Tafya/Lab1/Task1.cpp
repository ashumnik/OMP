#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

#include <cstdlib>
#include <ctime>

std::vector<std::string>* GetLines(std::istream* input);
std::vector<std::string>* LoadFile(std::string filename);
void ParseLines(std::vector<std::string>* lines);
void Analyze(std::string str_to_analyze);
std::map<int, class Node*> all_states;

std::queue<std::set<class Node*>> Q;
std::set<std::set<class Node*>> Q_history;
std::set<char> E;

void Determine();


struct Node{
    int state = -1;
    bool is_final = false;
    std::map<char, int> next_states;
    std::map<int, std::set<Node*>> states_to_nodes;
    
    static bool doesExist(int state);

    std::set<Node*> GetNextState(char ch){
        try{
            return states_to_nodes.at(next_states.at(ch));
        }
        catch(std::out_of_range e){
            return std::set<Node*>();
        }
    }

    void PrintNode(char ch){
        auto next_node = GetNextState(ch);

        std::cout << std::setw(6) << std::setfill(' ') << "q" << state;
        std::cout << "," << ch;
        std::cout << "=";
        std::cout << "(";
        for(auto state : next_node){
            std::cout << (is_final ? 'f' : 'q');
            std::cout << state->state;
        }
        std::cout << ")" ;
        std::cout << std::endl;
    }


} Root;

void Determine(){
    static bool is_first_call = true;

    //Шаг 1. Помещаем в очередь QQ множество, состоящее только из стартовой вершины.
    if(is_first_call){
        std::set<class Node*> node_set;
        node_set.insert(&Root);
        Q.push(node_set);
        Q_history.insert(node_set);
        is_first_call = false;
    }

    //Шаг 2. Затем, пока очередь не пуста выполняем следующие действия:
    while(!Q.empty()){
        //Достаем из очереди множество, назовем его q
        auto q = Q.front();
        Q.pop();

        for(auto state : q){
            std::set<Node*> new_state_set;

            //Для всех ch, принадлежащих E
            for(auto ch : E){
                // посмотрим в какое состояние ведет переход по символу ch из каждого состояния в q.
                auto state_set = state->GetNextState(ch);
                if(state_set.empty()){
                    continue;
                }
                else{
                    for(auto looked_up_state : state_set){
                        new_state_set.insert(looked_up_state);
                    }
                }
            }

            //Полученное множество состояний положим в очередь Q только если оно не лежало там раньше.
            if(!Q_history.count(new_state_set)){
                Q.push(new_state_set);
                Q_history.insert(new_state_set);
            }
        }
    }

}

bool Node::doesExist(int state){
    return all_states.count(state) == 1; 
}


int main (){
    auto lines = LoadFile("test.txt");
    ParseLines(lines);

    Root.PrintNode('a');
    std::cout << std::endl;

    Analyze("ba +c");
}

void Analyze(std::string str_to_analyze){
    auto first_node = &Root;
    std::set<Node*> current_node;
    current_node.insert(first_node);

    for(auto ch : str_to_analyze){

        for(auto node : current_node){
            if(node->next_states.count(ch) >= 1){
                std::cout << "Correct [" << ch << "] : "; 
                node->PrintNode(ch);
                current_node = node->GetNextState(ch);
                break;
            }
            else{
                std::cout << "Incorrect [" << ch << "]" << std::endl;
                std::cout << "String wrong string" << std::endl;
                goto exit;
            }
        }
    }
exit:
    return;
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
            current_node->states_to_nodes[next_state_number].insert(all_states[next_state_number]);
        }
        if(state_number == 0){
            Root = *current_node;
        }

    }
}
