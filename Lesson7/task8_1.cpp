#include "mpi.h"
#include <locale.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h> 

int main(int argc, char** argv)
{
	const int n = 4;
	double a[n][n], b[n][n], c[n][n];
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				a[i][j] = 1.0;
				b[i][j] = 2.0;
			}
		}

		int num = ceil((double)n / (size - 1));
		printf("from process %d \n", rank);
		for (int i = 1; i < size; i++) {
			int start_row = (i - 1) * num;

			if (n - start_row <= num) {
				num = n - start_row;
			}
			MPI_Send(&num, 1, MPI_INT, i, 5, MPI_COMM_WORLD);
			MPI_Send(&start_row, 1, MPI_INT, i, 10, MPI_COMM_WORLD);

			for (int j = start_row; j < start_row + num; j++) {
				MPI_Send(&a[j][0], n, MPI_DOUBLE, i, 11, MPI_COMM_WORLD);
				MPI_Send(&b[j][0], n, MPI_DOUBLE, i, 22, MPI_COMM_WORLD);
			}

			double buffer_c[n][n];
			MPI_Status status_c;

			for (int j = start_row; j < start_row + num; j++) {
				MPI_Recv(buffer_c[j], n, MPI_DOUBLE, i, 55, MPI_COMM_WORLD, &status_c);

				for (int k = 0; k < n; k++) {
					printf("array_c[%d][%d] = %.2f ", j, k, buffer_c[j][k]);
				}
				printf("\n");
			}
		}
	}

	else if (rank != 0) {
		int elements;
		int start;
		MPI_Status status_a;
		MPI_Status status_b;
		MPI_Status status_el;
		MPI_Status status_st;

		MPI_Recv(&elements, 1, MPI_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status_el);
		MPI_Recv(&start, 1, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &status_st);

		double buffer_a[n][n];
		double buffer_b[n][n];

		printf("from process = %d \n", rank);
		for (int i = start; i < start + elements; i++) {
			MPI_Recv(&buffer_a[i], n, MPI_DOUBLE, MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status_a);
			MPI_Recv(&buffer_b[i], n, MPI_DOUBLE, MPI_ANY_SOURCE, 22, MPI_COMM_WORLD, &status_b);

			for (int j = 0; j < n; j++) {
				printf("a[%d][%d] = %.2f ", i, j, buffer_a[i][j]);
			}
			printf("\n");

			for (int j = 0; j < n; j++) {
				printf("b[%d][%d] = %.2f ", i, j, buffer_b[i][j]);
			}

			printf("\n");

			for (int j = 0; j < n; j++) {
				c[i][j] = buffer_a[i][j] * buffer_b[i][j];
				printf("c[%d][%d] = %.2f ", i, j, c[i][j]);
			}
			printf("\n");

			MPI_Send(&c[i][0], n, MPI_DOUBLE, 0, 55, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
}