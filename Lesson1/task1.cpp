#include <iostream>

#include <iostream>
#include <omp.h>

int main() {
    int numThread = 8;
    omp_set_num_threads(numThread);
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Thread(%d): Hello World!\n", id);
    }
}
