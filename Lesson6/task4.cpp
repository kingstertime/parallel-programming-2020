#include "mpi.h"
#include <iostream>

int main(int argc, char** argv) {
	int rank, size, count;
	int a[10] = { 5,1,9,1,0,3,2,7,8,2 };
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 1) {
		for (int i = 0; i < size; i++) {
			if (i != 1) {
				MPI_Send(&a[0], 10, MPI_INT, i, 5, MPI_COMM_WORLD);
			}
		}
	}
	else {
		MPI_Probe(1, 5, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &count);
		int* a_temp = (int*)malloc(sizeof(int) * count);
		MPI_Recv(a_temp, 10, MPI_INT, 1, 5, MPI_COMM_WORLD, &status);
		printf("\nprocess %d got message: \n", rank);
		for (int i = 0; i < 10; i++) {
			printf("a[%d] = %d \n", i, a[i]);
		}
		free(a_temp);
	}
	MPI_Finalize();

	return 0;
}