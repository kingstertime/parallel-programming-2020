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

int main(int argc, char** argv) {

	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int integer = 0;
	const int n = 4, m = 4;

	int send_part = n * m / size;
	int* recvbuf = new int[send_part];
	int* sendcounts = new int[size];
	int* displs = new int[size];
	int a[n][m];

	if (rank == 0)
	{
		integer += send_part;
		for (int i = 0; i < n; i++)
			fill_array(a[i], m);

		for (int i = 0; i < size; i++)
		{
			sendcounts[i] = send_part;
			displs[i] = i * send_part;
		}
	}

	MPI_Scatterv(&a, sendcounts, displs, MPI_INT, recvbuf, send_part, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Finalize();
}