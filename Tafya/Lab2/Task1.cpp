
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <stack>
#include <deque>
#include <string>
#include <utility>
#include <vector>

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cctype>

std::map<char, int> Priority; 
void stackSwap(std::stack<struct StackOperation>& stack);
void stackSwapPush(std::stack<struct StackOperation>& stack, struct StackOperation op);
void ParseInput(std::string process);
void stackPopToOp();


struct AtomicActions{

    enum class Type{
        DIV,
        MUL,
        SUB,
        SUM,
        STR,
        POW
    };

    static double div(double l, double r){return l/r;}
    static double mul(double l, double r){return l*r;}
    static double sub(double l, double r){return l-r;}
    static double sum(double l, double r){return l+r;}
    static double str(double l, double r){return l  ;}
    static double pow(double l, double r){return pow(l,r);}

};

std::stack<double> global_stack;
std::deque<struct StackOperation> operation_stack;
std::stack<struct StackOperation> parse_operation_stack;

struct StackOperation{
    
    unsigned int frames_affected = 2;
    double (*op)(double, double);
    int priority;
    double storage;

    void ChooseOp(AtomicActions::Type op_type){
        switch(op_type){
            case AtomicActions::Type::DIV: op = AtomicActions::div; break;
            case AtomicActions::Type::MUL: op = AtomicActions::mul; break;
            case AtomicActions::Type::SUB: op = AtomicActions::sub; break;
            case AtomicActions::Type::SUM: op = AtomicActions::sum; break;
            case AtomicActions::Type::STR: op = AtomicActions::str; break;
            case AtomicActions::Type::POW: op = AtomicActions::pow; break;
        }
    }

    StackOperation(char op){
        switch(op){
            case '+': ChooseOp(AtomicActions::Type::SUM); priority = Priority['+']; break;
            case '-': ChooseOp(AtomicActions::Type::SUB); priority = Priority['-']; break;
            case '*': ChooseOp(AtomicActions::Type::MUL); priority = Priority['*']; break;
            case '/': ChooseOp(AtomicActions::Type::DIV); priority = Priority['/']; break;
            case 'r': ChooseOp(AtomicActions::Type::STR); priority = Priority['r']; frames_affected = 1; break;
            case 'u': ChooseOp(AtomicActions::Type::POW); priority = Priority['u']; break;
            case '(': priority = Priority['(']; parse_operation_stack.push(*this);  break;
            case ')': while(parse_operation_stack.top().priority != Priority[')']){
                        stackPopToOp();   
                      } 
                      parse_operation_stack.pop();

                      break;
        }
    }

    void operator()(){
        switch(this->frames_affected){
            case 1:
                {
                    auto val = global_stack.top();
                    global_stack.pop();
                    global_stack.push(this->op(val, val));
                }
                break;
            case 2:
                {
                    auto left = global_stack.top();
                    auto right = global_stack.top();
                    global_stack.pop();
                    global_stack.pop();
                    global_stack.push(this->op(left, right));
                }
                break;
        }
    }

};



void stackSwapPush(std::stack<StackOperation>& stack, StackOperation op){
    auto previous = stack.top();
    stack.pop();
    stack.push(op);
    stack.push(previous);
}

void stackSwap(std::stack<StackOperation>& stack){
    auto previous = stack.top();
    stackSwapPush(stack, previous);
}

void stackPopToOp(){
    operation_stack.push_back(parse_operation_stack.top());
    parse_operation_stack.pop();
}

void ParseInput(std::string process){
    std::regex find_func("pow");
    std::regex_replace(process,find_func,"u");

    //std::regex parse_regex("^(\\d*\\.?\\d*|\\+|-|\\*|\\/|\\(|\\)|u)");
    std::regex parse_regex(R"###(^(\d*\.?\d*|\+|-|\*|\/|\(|\)|u))###");

    while(!process.empty()){
        std::smatch match;
        std::regex_search(process, match, parse_regex);
        std::string out = match[1];
        std::string full_match = match[0];
        std::cout << process << " | " << full_match << " | " << out <<" | " << match.size() << std::endl;
        process = std::regex_replace(process, parse_regex, "");

        if(std::isdigit(out[0])){
            auto number = StackOperation('r');
            number.storage = std::stod(out); 
            operation_stack.push_back(number);
            continue;
        }

        parse_operation_stack.push(StackOperation(out[0]));
        
    }

    while(!parse_operation_stack.empty()){
        stackPopToOp();
    }
}

void Execute(){
    while(!operation_stack.empty()){
        operation_stack.front()();
        operation_stack.pop_front();    
    }
    std::cout << "result: " << global_stack.top() << std::endl;
}


int main(){

    Priority['r'] = 1;
    Priority['u'] = 2;
    Priority['+'] = 2;
    Priority['-'] = 2;
    Priority['*'] = 3;
    Priority['/'] = 3;
    Priority['('] = -1;
    Priority[')'] = -1;

    ParseInput("1+2");
    Execute();

    return 0;
}
