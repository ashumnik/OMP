//#include <omp.h>
//
#include <iostream>
#include <iomanip>
#include <bitset>
#include <chrono>
#include <vector>

#include <cstdlib>
#include <ctime>
#define OUTPUT
#define N 40
#define DESIRED_LF 20

#if N < DESIRED_LF
    #define LF N
#else
    #define LF DESIRED_LF
#endif

/* Склейка функции под вариант */
#define CHOOSE_VAR(X)(calculate(var_ ## X ## ## _op , var_ ## X ## ## _res))

int max(int a, int b);
int var_0_op(int a, int b);
int var_1_op(int a, int b);
int var_2_op(int a, int b);
int var_3_op(int a, int b, int c);
int var_4_op(int a, int b, int c);
void var_0_res(long long* res, int b);
void var_1_res(long long* res, int b);
void var_2_res(long long* res, int b);
void var_3_res(long long* res, int b);
void var_4_res(long long* res, int b);

void print_matrix(int matrix[N]);
long long populate_C(int (*op)(int,int));

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

    #pragma omp parallel for
    for(int i = 0; i < N; i++){
        matrix[i] = std::rand()%N+1;
    }
}


/*
 * Принимает операцию и функцию финального подсчета
 * Сначала над i-ми элементами
 * массивов A и B выполняется операция, потом финальный подсчет.
 * Если вариант >2, то заменить первый аргумент и не трогать массив C
 */
long long calculate(int (*op)(int,int), void (*final_calc)(long long*, int)){

    // Инициализировать в 1, если умножение и в 0, если сложение
    long long result = 1;
    long long partial_result = 1;

    // Инициализировать в 1, если умножение и в 0, если сложение
    #pragma omp parallel for
    for(int i = 0; i < N; i++){
        C[i] = op(A[i],B[i]);
    }
    
    // Изменить операцию над result на свой вариант
    #pragma omp parallel for 
    for(int i = 0; i < N; i++){
        #pragma omp atomic
        result *= final_calc(&partial result, C[i]);
    }
    return result;
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

    auto result = CHOOSE_VAR(1);

    end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
    std::cout << elapsed << " microseconds" << std::endl;
    std::cout << "result: " << result << std::endl;

}

/*
 * Операции над массивами
 *
 */

int max(int a, int b){
    return a < b ? b : a;
}

int var_0_op(int a, int b){
    return max(a,b);
}

int var_1_op(int a, int b){
    return a*b;
}

int var_2_op(int a, int b){
    int res = max(a + b, 4*a - b);
    return res > 1 ? res : 0;
}

int var_3_op(int a, int b, int c){
    return a%2 == 0 ? b/c : b+a;
}

int var_4_op(int a, int b, int c){
    int res = a%2 == 0 ? b/c : b+a;
    return res != 1 ? res : 0 ;
}


/*
 * Операции над результатом
 * 
 */

void var_0_res(long long* res, int b){
   *res += b;
}

void var_1_res(long long* res, int b){
   *res *= b != 0 ? b : 1;
}

void var_2_res(long long* res, int b){
   *res += b > 1 ? b : 0; 
}

void var_3_res(long long* res, int b){
   *res *= b != 0 ? b : 1;
}

void var_4_res(long long* res, int b){
   *res += b;
}
