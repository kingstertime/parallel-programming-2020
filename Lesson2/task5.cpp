#include <iostream>
#include <omp.h>
using namespace std;

//int main() {
//
//    int a;
//    srand(time(NULL));
//    a = rand() % 30;
//    cout << (a) << endl;
//}

int main() {
    int d[6][8];
    srand(time(NULL));
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            d[i][j] = rand() % 30;
            printf("%d ", d[i][j]);
        }

        printf("\n");
    }

    printf("\n");

#pragma omp parallel sections
    {
#pragma omp section
        {
            int elements = 0;
            int sum = 0;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    sum += d[i][j];
                    elements++;
                }
            }
            printf("mid = %.2f from %d thread\n", sum * 1.0 / elements, omp_get_thread_num());
        }

#pragma omp section
        {
            int min = 30 + 1;
            int max = -1;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    if (max > d[i][j]) {
                        max = d[i][j];
                    }
                    if (min < d[i][j]) {
                        min = d[i][j];
                    }
                }
            }
            printf("max = %d max = %d from %d thread\n", max, min, omp_get_thread_num());
        }

#pragma omp section
        {
            int sum = 0;
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    if (d[i][j] % 3 == 0) {
                        sum += d[i][j];
                    }
                }
            }
            printf("sum of deviders = %d from %d thread\n", sum, omp_get_thread_num());
        }
    }
}
