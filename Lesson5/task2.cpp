#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("num of processes: %d, current num = %d\n", size, rank);
	MPI_Finalize();

	return 0;
}