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

	int partSize = 0;
	int localsum = 0;
	int globalsum = 0;
	const int n = 30;

	partSize = n / size;
	int* sendbuf = new int[n];
	int* getbuf = new int[partSize];

	fill_array(sendbuf, 30);

	MPI_Scatter(sendbuf, partSize, MPI_INT, getbuf, partSize, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < partSize; i++) {
		localsum += abs(getbuf[i]);
	}

	printf("localsum = %d \n", localsum);

	MPI_Reduce(&localsum, &globalsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("global sum = %d \n", globalsum);
	}

	MPI_Finalize();
}