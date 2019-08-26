//#include <omp.h>
//
#include <iostream>
#include <iomanip>
#include <bitset>
#include <chrono>

#include <cstdlib>
#include <ctime>
#define OUTPUT
#define PARALLEL
#define N 40
#define LF 20

int A[N] = {0};
int B[N] = {0};
int C[N] = {0};

void print_matrix(int matrix[N]){
    #ifdef OUTPUT
    for(int k = 0; k < N ; k+=LF){
        std::cout << std::setw(3) << "";

        for(int i = k; i < k+LF; i++){
            std::cout << std::setw(5) << std::setfill('_') << i << '|';
        }
        std::cout << std::endl;

        std::cout << std::setfill(' ');
        std::cout << std::setw(3) << "";

        for(int i = k; i < k+LF; i++){
            std::cout << std::setw(5) << matrix[i] << '|';
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
    #endif
    
}
void init(int matrix[N]){
    std::srand(unsigned(std::time(0)));
#ifdef PARALLEL
    #pragma omp parallel for
#endif
    for(int i = 0; i < N; i++){
        matrix[i] = std::rand()%N+1;
    }
}

int main(){
    init(A);
    init(B);

    std::cout << std::setw(3) << "";
    std::cout << "vvvvv A vvvvv" << std::endl;

    print_matrix(A);

    std::cout << std::setw(3) << "";
    std::cout << "vvvvv B vvvvv" << std::endl;

    print_matrix(B);

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();


    end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
    std::cout << elapsed << " microseconds" << std::endl;

    

}
