
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include <cstdlib>
#include <ctime>
#include <cmath>

int Priority[] = {
    ['u'] = 2,
    ['+'] = 2,
    ['-'] = 2,
    ['*'] = 3,
    ['/'] = 3,
    ['('] = 1,
    [')'] = 1
};

struct AtomicActions{

    enum class Type{
        DIV,
        MUL,
        SUB,
        SUM,
        POW
    };

    static double div(double l, double r){return l/r;}
    static double mul(double l, double r){return l*r;}
    static double sub(double l, double r){return l-r;}
    static double sum(double l, double r){return l+r;}
    static double pow(double l, double r){return pow(l,r);}

};

std::stack<double> global_stack;
struct StackOperation{
    
    unsigned int frames_affected = 2;
    double (*op)(double, double);

    void ChooseOp(AtomicActions::Type op_type){
        switch(op_type){
            case AtomicActions::Type::DIV: op = AtomicActions::div; break;
            case AtomicActions::Type::MUL: op = AtomicActions::mul; break;
            case AtomicActions::Type::SUB: op = AtomicActions::sub; break;
            case AtomicActions::Type::SUM: op = AtomicActions::sum; break;
            case AtomicActions::Type::POW: op = AtomicActions::pow; break;
        }
    }

    Operation(char op){
        switch(op){
            case '+': ChooseOp(AtomicActions::Type::SUM); break;
            case '-': ChooseOp(AtomicActions::Type::SUB); break;
            case '*': ChooseOp(AtomicActions::Type::MUL); break;
            case '/': ChooseOp(AtomicActions::Type::DIV); break;
            case 'u': ChooseOp(AtomicActions::Type::POW); break;


        }
    }

    void operator()(){
        auto left = global_stack.top();
        auto right = global_stack.top();
        global_stack.pop();
        global_stack.pop();
        global_stack.push(this->op(left, right));
    }

};
std::stack<StackOperation> operation_stack;

void ParseInput(std::string process){
    std::regex find_func("pow");
    std::regex_replace(process,find_func,"u");
    std::stack<StackOperation> operation_stack;
    for(auto ch : process){
        if(ch == )
        Priority[ch]

    }    

}



int main(){


    return 0;
}
