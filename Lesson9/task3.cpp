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

	int x = rand();
	int	sum = 0;
	double starttime, endtime, duration, maxduration;

	starttime = MPI_Wtime();

	MPI_Reduce(&x, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Bcast(&sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

	endtime = MPI_Wtime();
	duration = endtime - starttime;
	MPI_Reduce(&duration, &maxduration, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("Reduce + Bcast: %f\n", maxduration);

	sum = 0;

	starttime = MPI_Wtime();
	starttime = MPI_Wtime();

	MPI_Allreduce(&x, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	endtime = MPI_Wtime();
	duration = endtime - starttime;
	MPI_Reduce(&duration, &maxduration, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("Allreduce: %f\n", maxduration);

	MPI_Finalize();
}