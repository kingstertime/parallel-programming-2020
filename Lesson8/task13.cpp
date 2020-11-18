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

	int arrayA[n][n];
	int arrayB[n][n];
	for (int i = 0; i < n; i++)
	{
		fill_array(arrayA[i], n);
		fill_array(arrayB[i], n);
	}

	int getbufA[n];
	int getbufB[n];

	int arrayC[n][n];
	int bufC[n];

	MPI_Scatter(arrayA, n, MPI_INT, getbufA, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(arrayB, n, MPI_INT, getbufB, n, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++) {
		bufC[i] = getbufA[i] * getbufB[i];
	}

	MPI_Gather(bufC, n, MPI_INT, arrayC, n, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("MATRIX C \n");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				printf("%d ", arrayC[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Finalize();
}