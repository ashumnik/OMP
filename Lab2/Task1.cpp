//#include <omp.h>
//
#include <iostream>
#include <iomanip>
#include <bitset>
#include <chrono>

#include <cstdlib>
#include <ctime>
#include <omp.h>

const int NMAX=100;
const int LIMIT=10;

void main (){
    int i, j;
    float sum;
    float a[NMAX][NMAX];
    for (i=0; i < NMAX; i++)
        for (j=0; j < NMAX; j++)
            a[i][j] = i+j;

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel shared(a) if (NMAX>LIMIT)
    {
        #pragma omp for private(i,j,sum)
        for (i=0; i < NMAX; i++){
            sum = 0;
            for (j=0; j < NMAX; j++)
            sum += a[i][j];
            printf ("Сумма элементов строки %d равна %f\n",i,sum);
        }
    } /* Завершение параллельного фрагмента */

    end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
    std::cout << elapsed << " microseconds" << std::endl;
}
