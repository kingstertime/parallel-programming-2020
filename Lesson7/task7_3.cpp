#include "mpi.h"
#include <locale.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <array>

void fill_array(int* array, long int length, int seed = 0)
{
	srand(clock() + seed);

	for (int i = 0; i < length; i++)
		array[i] = (rand() % 1000) - 500;
}


int main(int argc, char** argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n = 11;

	if (rank == 0) {
		int x[11];
		fill_array(x, 11);
		int y[11];
		fill_array(y, 11);

		int size_ = ceil((double)n / (size - 1));

		for (int i = 1; i < size; i++) {

			int start = (i - 1) * size_;

			if (n - start <= size_) {
				size_ = n - start;
			}

			MPI_Send(&start, 1, MPI_INT, i, 10, MPI_COMM_WORLD);
			MPI_Send(x + start, size_ < 0 ? 0 : size_, MPI_INT, i, 11, MPI_COMM_WORLD);
			MPI_Send(y + start, size_ < 0 ? 0 : size_, MPI_INT, i, 12, MPI_COMM_WORLD);
		}

		printf("from process %d \n", rank);
		for (int i = 1; i < size; i++) {

			printf("process %d \n", i);

			int* buffer_x;
			int* buffer_y;
			int start;
			int elements;

			MPI_Status status_x;
			MPI_Status status_y;
			MPI_Status status_start;
			MPI_Status status_el;

			MPI_Recv(&start, 1, MPI_INT, i, 31, MPI_COMM_WORLD, &status_start);
			MPI_Recv(&elements, 1, MPI_INT, i, 32, MPI_COMM_WORLD, &status_el);
			buffer_x = new int[elements];
			buffer_y = new int[elements];

			MPI_Recv(buffer_x, elements, MPI_INT, i, 21, MPI_COMM_WORLD, &status_x);
			MPI_Recv(buffer_y, elements, MPI_INT, i, 22, MPI_COMM_WORLD, &status_y);

			printf("start = %d \n", start);

			for (int j = 0; j < elements; j++) {
				x[j] = buffer_y[j];
				printf("x[%d] = %d ", j, x[j]);
			}
			printf("\n");

			for (int j = 0; j < elements; j++) {
				y[j] = buffer_x[j];
				printf("y[%d] = %d ", j, y[j]);
			}
			printf("\n");
		}
		printf("\n");
	}

	else {
		int elements_x = 0;
		int start;

		int* buffer_x;
		int* buffer_y;

		MPI_Status status_x;
		MPI_Status status_y;
		MPI_Status status_start;

		MPI_Recv(&start, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &status_start);

		MPI_Probe(MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status_x);
		MPI_Get_count(&status_x, MPI_INT, &elements_x);
		buffer_x = new int[elements_x];
		MPI_Recv(buffer_x, elements_x, MPI_INT, MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status_x);

		MPI_Probe(MPI_ANY_SOURCE, 12, MPI_COMM_WORLD, &status_y);
		MPI_Get_count(&status_y, MPI_INT, &elements_x);
		buffer_y = new int[elements_x];
		MPI_Recv(buffer_y, elements_x, MPI_INT, MPI_ANY_SOURCE, 12, MPI_COMM_WORLD, &status_y);

		printf("process %d: \n", rank);

		//x
		for (int i = 0; i < elements_x; i++) {
			printf(" x : %d ", buffer_x[i]);
		}
		printf("\n");

		//y
		for (int i = 0; i < elements_x; i++) {
			printf(" y : %d ", buffer_y[i]);
		}
		printf("\n");

		MPI_Send(&start, 1, MPI_INT, 0, 31, MPI_COMM_WORLD);
		MPI_Send(&elements_x, 1, MPI_INT, 0, 32, MPI_COMM_WORLD);
		MPI_Send(buffer_x, elements_x, MPI_INT, 0, 21, MPI_COMM_WORLD);
		MPI_Send(buffer_y, elements_x, MPI_INT, 0, 22, MPI_COMM_WORLD);

	}
	MPI_Finalize();
}