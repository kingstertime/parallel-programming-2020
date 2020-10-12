#include "mpi.h"
#include <iostream>

int main(int argc, char **argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("processes: %d, Hello, World! \n", rank);
	MPI_Finalize();
}
