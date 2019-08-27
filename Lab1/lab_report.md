---
title: Отчет по лабораторной работе №1
author: Уманский Александр, ПИН-43
---
# Вариант X

## Задание 1
Написать на C++ программу в соответствии с вариантом задания, при этом программы
должны оптимально использовать предлагающиеся вычислительные ресурсы. Для реализации
многопоточности использовать технологию OpenMP. *Измерять время работы программы для
различных значений параметров*. **Результаты занести в отчёт.** Также, при запуске программ,
запустить диспетчер задач Windows, в нём установить счётчик потоков, и *просмотреть сколько
потоков выполнения использует процесс*, соответствующий запущенной программе. **Результаты
также занести в отчёт.**

Текст задания варианта


Параллельно


|  N  |  M  |Количество потоков |Время обработки    |
|-----|-----|-------------------|-------------------|
| 100 | 100 |         1         |микросекунд        |
|     |     |                   |                   |
| 100 | 100 |         1         |микросекунд        |
|     |     |                   |                   |
| 100 | 100 |         1         |микросекунд        |
|     |     |                   |                   |
 
## Задание 2
Модифицировать программу, составленную по Заданию 1, убрав возможность
параллельного использования вычислительных ресурсов. Измерять время работы программы для
тех же значений параметров, что были использованы при выполнении Задания 1. Результаты
сравнить и занести в отчёт.

Последовательно


|  N  |  M  |Количество потоков |Время обработки    |
|-----|-----|-------------------|-------------------|
| 100 | 100 |         1         |микросекунд        |
|     |     |                   |                   |
| 100 | 100 |         1         |микросекунд        |
|     |     |                   |                   |
| 100 | 100 |         1         |микросекунд        |
|     |     |                   |                   |


## Вывод: 
Параллелизация дает выигрыш в производительности 
по сравнению с последовательным
исполнением программы в ситуациях, 
когда "стоимость" создания пула потоков меньше, чем последовательная обработка данных.
Чем разница больше, тем различие существеннее