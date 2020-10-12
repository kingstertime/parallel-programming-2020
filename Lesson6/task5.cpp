#include<mpi.h> 
#include<iostream> 
using namespace std;

int main(int argc, char **argv)
{
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		int a[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
		int block = 12 / size + 1; 
		int elements = 12 - block;
		for (int i = 1; i < size; i++)
		{
			if (elements > 0 && elements < block)
				block = elements;

			MPI_Send(&a[(12 / size + 1)*i], elements > 0 ? block : 0, MPI_INT, i, 13, MPI_COMM_WORLD);
			elements = elements - block;
		}
		printf("process 0: ");
		block = 12 / size + 1;
		for (int i = 0; i < block; i++)
		{
			printf("%d ", a[i]);
		}
	}
	else {
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int count;
		MPI_Get_count(&status, MPI_INT, &count);
		int *m = new int[count];

		MPI_Recv(m, count, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("process %d: ", rank);
		for (int i = 0; i < count; i++)
		{
			printf("%d ", m[i]);
		}
		delete[] m;
	}
	MPI_Finalize();
}