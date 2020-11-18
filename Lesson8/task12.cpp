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
	const int m = 4;

	int localsum = 0;
	int globalMax = 0;

	int array[m][n] =
	{
		{13,4,23,-4},
		{0,-2,-3,8},
		{-2,11,-3,-6},
		{-1,9,7,2}
	};

	int sendbuf[m][n];
	int getbuf[n];

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			sendbuf[i][j] = array[i][j];
			printf("%d ", sendbuf[i][j]);
		}
		printf("\n");
	}


	MPI_Scatter(sendbuf, n, MPI_INT, getbuf, n, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++) {
		localsum += abs(getbuf[i]);
	}

	printf("process = %d, localsum = %d \n", rank, localsum);

	MPI_Reduce(&localsum, &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("max = %d \n", globalMax);
	}

	MPI_Finalize();
}