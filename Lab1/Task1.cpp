//#include <omp.h>
//
#include <iostream>
#include <iomanip>
#include <bitset>
#include <chrono>

#include <cstdlib>
#include <ctime>
#define OUTPUT
#define M 10
#define N 20
#define N_h N/2

int matrix[M][N];
int matrix_computed[M/2][N/2] = 0;

int mul(int a, int b){
    return a*b;
}

int sum(int a, int b){
    return a+b;
}

long long bits(int a){
    return std::bitset<sizeof(decltype(matrix[0][0]))*8>(a).count();
}

long long sevens(int a){
    long long count = 0;

    while(a > 0){
        if(a % 10 == 7){
            count++;
        }
        a /= 10;
    }
    
    return count;
}

void init(){
    std::srand(unsigned(std::time(0)));
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
             matrix[i][j] = std::rand()%M+1;
             #ifdef OUTPUT
             std::cout << std::setw(5) << matrix[i][j];
             #endif
        }
        #ifdef OUTPUT
        std::cout << std::endl;
        #endif
    }
}

long long compute(int (op*)(int,int), long long (amount)(int)){
    long long result = 0;
    for(int i = 0; i < M; i++){
        for(int j = 0, j_c = 0; j < N_h; j+=2, j_c++){
             matrix_computed[i][j_c] = op(matrix[i][j], matrix[i][j+1]); 
             // atomic
             result += amount(matrix_computed[i][j_c]);
        }
    }
}

int main(){
    init();
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    start = std::chrono::high_resolution_clock.now()

    compute(mul,bits);

    auto elapsed = start.time_since_epoch();
    std::cout << elapsed.count() << "nanoseconds" << std::endl;

    

}
