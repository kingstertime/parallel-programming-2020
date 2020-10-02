//
// Created by Кирилл Королёв on 02.10.2020.
//

#include <iostream>
#include <omp.h>

int main() {
    int a[10] = { 42, 2, 3, 4, 91, 6, 5, 52, 43, 12 };
    int b[10] = { 17, 2, 13, 43, 65, 52, 35, 4, 23, 21 };

    int min = 10;
    int max = 0;

#pragma omp parallel sections num_threads(2)
    {
#pragma omp section
        {
            for (int i = 0; i < 10; i++) {
                if (max < a[i]) { max = a[i]; }
            }
            printf("max = %d, min %d\n", max, min);
        }

#pragma omp section
        {
            for (int i = 0; i < 10; i++) {
                if (min > b[i]) { min = b[i]; }
            }
            printf("max = %d, min %d\n", max, min);
        }
    }

//    printf("max = %d, min %d\n", max, min);

}