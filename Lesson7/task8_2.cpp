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
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		int a[m][n] =
		{
			{1, 2},
			{3, 4},
			{5, 6}
		};

		int b[n][m] =
		{
			{1, 2, 3},
			{4, 5, 6}
		};

		int num_of_m = ceil((double)m / (size - 1));

		printf("from process %d \n", rank);
		for (int i = 1; i < size; i++) {
			int start_m = (i - 1) * num_of_m;
			if (m - start_m <= num_of_m) {
				num_of_m = m - start_m;
			}
			start_m = (i - 1) * num_of_m;
			MPI_Send(&b[0][0], n * m, MPI_INT, i, 12, MPI_COMM_WORLD);
			for (int j = start_m; j < start_m + num_of_m; j++) {
				MPI_Send(&a[j], n, MPI_INT, i, 11, MPI_COMM_WORLD);
			}
		}
		for (int i = 1; i < size; i++) {
			int* buf_c = new int[m];
			MPI_Status status;
			for (int j = 0; j < m; j++) {
				buf_c[j] = 0;
			}
			MPI_Recv(buf_c, m, MPI_INT, i, 100, MPI_COMM_WORLD, &status);
			for (int j = 0; j < m; j++) {
				printf("c = %d ", buf_c[j]);
			}
			printf("\n");
		}
	}

	else if (rank != 0) {

		MPI_Status status_a;
		MPI_Status status_b;

		int* buffer_a = new int[n];
		int buffer_b[n][m];
		int mult[m];

		printf("from process = %d \n", rank);

		MPI_Recv(buffer_a, n, MPI_INT, 0, 11, MPI_COMM_WORLD, &status_a);
		MPI_Recv(&buffer_b, n * m, MPI_INT, 0, 12, MPI_COMM_WORLD, &status_b);

		for (int j = 0; j < n; j++) {
			printf("a[%d] = %d ", j, buffer_a[j]);
		}
		printf("\n");

		for (int j = 0; j < n; j++) {
			for (int i = 0; i < m; i++) {
				printf("b[%d][%d] = %d ", j, i, buffer_b[j][i]);
			}
			printf("\n");
		}

		for (int i = 0; i < m; i++) {
			mult[i] = 0;
		}

		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				mult[i] += buffer_a[j] * buffer_b[j][i];
			}
			printf("mult[%d] = %d ", i, mult[i]);
		}

		MPI_Send(&mult, m, MPI_INT, 0, 100, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}
