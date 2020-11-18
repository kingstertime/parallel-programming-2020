#include "mpi.h"
#include <locale.h>
#include <iostream>
#include <stdio.h>
#include <time.h>

void fill_array(int* array, long int length, int seed = 0)
{
	srand(clock() + seed);

	for (int i = 0; i < length; i++)
		array[i] = (rand() % 1000) - 500;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int n = 4;

	int local = 0;
	int array[n][n];
	for (int i = 0; i < n; i++)
	{
		fill_array(array[i], n);
	}

	int x[n];
	fill_array(x, n);

	int z[n];
	int getbuf[n];

	MPI_Scatter(array, n, MPI_INT, getbuf, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(x, n, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++) {
		local += getbuf[i] * x[i];
	}

	printf("localVal = %d from process %d \n", local, rank);

	MPI_Gather(&local, 1, MPI_INT, z, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("Z = ");
		for (int i = 0; i < n; i++) {
			printf("%d ", z[i]);
		}
		printf("\n");
	}

	MPI_Finalize();
}