#include<mpi.h>
#include<iostream>

int main(int argc, char** argv) {
	int rank, size;
	const int N = 10;
	int a[N];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			a[i] = rand() % 100 + 1;
		}
		MPI_Send(&a, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	if (rank == 1) {
		MPI_Recv(&a, N, MPI_INT, 0,
			MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		printf("Got\n");
		for (int i = 0; i < N; i++) {
			printf("%d ", a[i]);
		}
	}
	MPI_Finalize();

	return 0;
}