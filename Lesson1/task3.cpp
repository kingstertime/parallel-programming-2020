//
// Created by Кирилл Королёв on 02.10.2020.
//

#include <iostream>
#include <omp.h>

int main() {
    int a, b;
    a = 11;
    b = 2;

    printf("Before first parallel : a = %d; b = %d\n", a, b);

    omp_set_num_threads(2);
#pragma omp parallel private(a) firstprivate(b)
    {
        int thread_num = omp_get_thread_num();

        a += omp_get_thread_num();
        b += omp_get_thread_num();

        printf("Inside first parallel at %d thread: a = %d; b = %d\n", thread_num, a, b);

    }
    printf("After first parallel: a = %d; b = %d\n", a, b);

    omp_set_num_threads(4);
#pragma omp parallel shared(a) private(b)
    {

        int thread_num = omp_get_thread_num();

        a -= omp_get_thread_num();
        b -= omp_get_thread_num();

        printf("Inside second parallel at %d thread: a = %d; b = %d\n", thread_num, a, b);

    }
    printf("After second parallel: a = %d; b = %d\n", a, b);
}
