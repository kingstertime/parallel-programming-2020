#include <iostream>
#include "mpi.h"

int main(int argc, char** argv) {
	MPI_Status status;
	int rank, size, message_size;
	int const count = 18;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		int x[count], y[count];
		printf("Vector\nx y\n");
		for (int i = 0; i < count; i++) {
			x[i] = rand() % 100;
			printf("%d ", x[i]);
			y[i] = rand() % 100;
			printf("%d\n", y[i]);
		}
		for (int i = 1; i < size; i++) {
			MPI_Send(x + (i - 1) * count / (size - 1), count / (size - 1), MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(y + (i - 1) * count / (size - 1), count / (size - 1), MPI_INT, i, 2, MPI_COMM_WORLD);
		}
	} else {
		MPI_Probe(0, 1, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &message_size);
		int* recv_x = (int*)malloc(sizeof(int) * message_size);
		MPI_Recv(recv_x, message_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		MPI_Probe(0, 2, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &message_size);
		int* recv_y = (int*)malloc(sizeof(int) * message_size);
		MPI_Recv(recv_y, message_size, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

		int* z = (int*)malloc(sizeof(int) * message_size);
		for (int i = 0; i < message_size; i++) {
			z[i] = recv_x[i] + recv_y[i];
		}
		MPI_Send(z, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	if (rank == 0) {
		int result[count];
		int pointer = 0;

		for (int i = 1; i < size; i++) {
			MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &message_size);
			int* z = (int*)malloc(sizeof(int) * message_size);
			MPI_Recv(z, message_size, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			for (int j = 0; j < message_size; j++) {
				result[pointer] = z[j];
				pointer++;
			}
		}

		printf("Result z vector is : \n");
		for (int i = 0; i < count; i++) {
			printf("%d ", result[i]);
		}
		printf("\n");
	}

	MPI_Finalize();
}
