//
// Created by Кирилл Королёв on 28.09.2020.
//

#include <iostream>
#include "omp.h"

using namespace std;

int main()
{
    const int NUMBER = 210;
    int result = 0, odd = 1;

    omp_set_num_threads(4);

#pragma omp parallel
    {
#pragma omp for schedule(static)
        for (int i = 0; i < NUMBER; i++)
        {
            result += odd;
            odd += 2;
            printf("tread = %d, result = %d\n", omp_get_thread_num(), result);
        }
    }
    printf("result = %d", result);
}
