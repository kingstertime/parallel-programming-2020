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

	int* a_sbuff = new int[4];

	if (rank == 0)
	{
		int finnalArray[12];
		MPI_Status status;
		for (int i = 1; i < size; i++)
		{
			MPI_Recv(&finnalArray[4 * (i - 1)], 4, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
		}
		for (int i = 0; i < 12; i++)
		{
			printf("finnalArray[%d] = %d\n", i, finnalArray[i]);
		}
	}
	else
	{
		for (int i = 0; i < size - 1; i++)
		{
			fill_array(a_sbuff, 4);
			MPI_Send(a_sbuff, 4, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
		for (int i = 0; i < 4; i++)
		{
			printf("a[%d] = %d, from proc %d \n", i, a_sbuff[i], rank);
		}
	}
	MPI_Finalize();
}