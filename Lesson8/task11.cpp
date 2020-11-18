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

	int localsum = 0;
	int globalsum = 0;

	int partSize = 30 / size;
	int* vectorX = new int[30];
	int* recieveBuffer = new int[partSize];
	int vectorY[30];

	fill_array(vectorX, 30);
	fill_array(vectorY, 30);

	MPI_Scatter(vectorX, partSize, MPI_INT, recieveBuffer, partSize, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < partSize; i++) {
		localsum += vectorY[i] * recieveBuffer[i];
	}

	printf("localpartXY = %d \n", localsum);

	MPI_Reduce(&localsum, &globalsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("total vector = %d \n", globalsum);
	}

	MPI_Finalize();
}