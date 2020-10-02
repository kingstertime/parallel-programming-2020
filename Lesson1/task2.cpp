//
// Created by Кирилл Королёв on 02.10.2020.
//
#include <iostream>
#include <omp.h>

int main() {
    omp_set_dynamic(0);

    int num_threads = 3;
    omp_set_num_threads(num_threads);

#pragma omp parallel if (num_threads > 1)
    if (num_threads > 1)
    {
        printf("thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads() - 1);
    }

    num_threads = 1;
    omp_set_num_threads(num_threads);

#pragma omp parallel if (num_threads > 1)
    if (num_threads > 1)
    {
        printf("thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads() - 1);
    }
}
