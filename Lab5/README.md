# Отчет по лабораторной работе №5

В данной лабораторной работе
был модифицирован код лабораторных работ №3 и №4.

Время исполнения программ измерялось по всей функции `main()`.
В качестве сегмента для распараллеливания по задачам 
были выбраны вызовы функций инициализации массивов:

### Лабораторная 3

~~~
int main(){

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel sections
    {
        #pragma omp section
        { init(A); }
        #pragma omp section
        { init(B); }
    }

    auto result = CHOOSE_VAR(1);

    end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
    std::cout << elapsed << " microseconds" << std::endl;
}
~~~

### Лабораторная 4

~~~
int main(){

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    init(A);
    init(B);

    auto result = CHOOSE_VAR(1);

    end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
    std::cout << elapsed << " microseconds" << std::endl;

}
~~~

## Замеры времени:

| Количество элементов | Лаб №3 (мс)     | Лаб №4 (мс)     | Отношение времени |
| ---------------      | --------------- | --------------- | --------------    |
| 1000000              | 324454          | 852798          | 2.628             |
| 100000               | 21916           | 40646           | 1.854             |
| 10000                | 12197           | 5293            | 0.433             |
| 1000                 | 7171            | 870             | 0.121             |

### Вывод

Видно, что в среднем, выигрыш от параллелизации 
приводит к повышению производительности, 
равному количеству независимых задач. 
Потери могут наблюдаться из-за затрат на создание потоков и 
ождания завершения работы всех секций.

Необычное ускорение последовательного куска кода, на малых количествах элементов, 
так же связан с оптимизациями компилятора g++, которые невозможно полностью отключить. 
С этим же, скорее всего, связано и нелинейное ускорение работы программы на большом количестве элементов.
