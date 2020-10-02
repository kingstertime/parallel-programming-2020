#include <iostream>
#include <math.h>
#include "omp.h"

using namespace std;

int main()
{
    const int SIZE = 30;
    int a[SIZE] = { 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1 };

    omp_set_num_threads(4);

    int sum = 0, degree = 0;
    #pragma omp parallel shared(a)
    {
    #pragma omp for schedule(static)
    for (int i = SIZE - 1; i >= 0; i--)
    {
        sum += a[i] * pow(2, degree);
        degree++;
        printf("tread = %d, result = %d\n", omp_get_thread_num());
    }
    }
    printf("Number = %d", sum);
}