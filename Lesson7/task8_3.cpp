#include "mpi.h"
#include <locale.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h> 

int main(int argc, char** argv)
{
	const int n = 2;
	const int m = 3;
	double c[n][m];
	double vector[n];
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {

		double a[m][n] =
		{
			{1, 2},
			{3, 4},
			{5, 6}
		};


		int num_of_m = ceil((double)m / (size - 1));

		printf("from process %d \n", rank);
		for (int i = 1; i < size; i++) {

			int start_m = (i - 1) * num_of_m;

			if (m - start_m <= num_of_m) {
				num_of_m = m - start_m;
			}
			start_m = (i - 1) * num_of_m;

			MPI_Send(&num_of_m, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
			MPI_Send(&start_m, 1, MPI_INT, i, 4, MPI_COMM_WORLD);

			for (int j = start_m; j < start_m + num_of_m; j++) {
				MPI_Send(&a[j][0], n, MPI_DOUBLE, i, 11, MPI_COMM_WORLD);
			}

		}

		for (int i = 1; i < size; i++) {

			printf("process %d \n", i);

			MPI_Status status_c;
			MPI_Status status;
			MPI_Status status_num;
			int start;
			int num;

			double buf_vector[n];

			MPI_Recv(&start, 1, MPI_INT, i, 5, MPI_COMM_WORLD, &status);
			MPI_Recv(&num, 1, MPI_INT, i, 7, MPI_COMM_WORLD, &status_num);
			MPI_Recv(&buf_vector, n, MPI_DOUBLE, i, 12, MPI_COMM_WORLD, &status_c);


			for (int j = start; j < start + num; j++) {

				for (int l = 0; l < n; l++) {
					c[l][j] = buf_vector[l];
					printf("c[%d][%d] = %.2f ", l, j, c[l][j]);
				}

				printf("\n");
			}

		}

		printf("\n");
	}

	else if (rank != 0) {
		int elements;
		int start;
		MPI_Status status_a;
		MPI_Status status_el;
		MPI_Status status_st;

		MPI_Recv(&elements, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status_el);
		MPI_Recv(&start, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status_st);

		double buffer_a[m][n];

		printf("from process = %d \n", rank);
		for (int i = start; i < start + elements; i++) {
			MPI_Recv(&buffer_a[i][0], n, MPI_DOUBLE, MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status_a);

			for (int j = 0; j < n; j++) {
				printf("a[%d][%d] = %.2f ", i, j, buffer_a[i][j]);
				vector[j] = buffer_a[i][j];
			}
			printf("\n");

			for (int j = 0; j < n; j++) {
				printf("vector[%d] = %.2f ", j, vector[j]);
			}

			printf("\n");

		}

		MPI_Send(&start, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
		MPI_Send(&elements, 1, MPI_INT, 0, 7, MPI_COMM_WORLD);
		MPI_Send(&vector, n, MPI_DOUBLE, 0, 12, MPI_COMM_WORLD);

		printf("\n");
	}

	MPI_Finalize();
}