//#include <omp.h>
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#define OUTPUT
#define M 10
#define N 20
#define N_h N/2

int matrix[M][N];
int matrix_computed[M/2][N/2] = 0;

void init(){
    std::srand(unsigned(std::time(0)));
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
             matrix[i][j] = std::rand()%M;
             #ifdef OUTPUT
             std::cout << std::setw(5) << matrix[i][j];
             #endif
        }
        #ifdef OUTPUT
        std::cout << std::endl;
        #endif
    }
}

void compute(int (op*)(int,int)){
    for(int i = 0; i < M; i++){
        for(int j = 0, j_c = 0; j < N_h; j+=2, j_c++){
             matrix_computed[i][j_c] = op(matrix[i][j], matrix[i][j+1]); 
        }
    }
}

int main(){
    init();
    

}
