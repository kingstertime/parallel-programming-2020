#include <iostream>
#include <mpi.h>
#include <cmath>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int size = 2;
    int N = 3;
    int groupOfProcesses1[N], groupOfProcesses2[N], groupOfProcesses3[N], groupOfProcesses4[N];

    int world_rank;
    int world_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Group worldGroup;
    MPI_Group newGroup1, newGroup2, newGroup3, newGroup4;
    MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);

    int index1 = 0, index2 = 0, index3 = 0, index4 = 0;
    for (int i = 0; i < world_size; ++i) {
        if (i >= 0 && i < 3){
            groupOfProcesses1[index1] = i;
            index1++;
        }
        if (i >= 3 && i < 6){
            groupOfProcesses2[index2] = i;
            index2++;
        }
        if (i >= 6 && i < 9){
            groupOfProcesses3[index3] = i;
            index3++;
        }
        if (i >= 9 && i < 12){
            groupOfProcesses4[index4] = i;
            index4++;
        }
    }

    MPI_Group_incl(worldGroup, N, groupOfProcesses1, &newGroup1);
    MPI_Group_incl(worldGroup, N, groupOfProcesses2, &newGroup2);
    MPI_Group_incl(worldGroup, N, groupOfProcesses3, &newGroup3);
    MPI_Group_incl(worldGroup, N, groupOfProcesses4, &newGroup4);

    MPI_Comm newComm1, newComm2, newComm3, newComm4;
    MPI_Comm_create(MPI_COMM_WORLD, newGroup1, &newComm1);
    MPI_Comm_create(MPI_COMM_WORLD, newGroup2, &newComm2);
    MPI_Comm_create(MPI_COMM_WORLD, newGroup3, &newComm3);
    MPI_Comm_create(MPI_COMM_WORLD, newGroup4, &newComm4);

    int a[size];
    int a_received[size * N];

    int newRank;

    if (newComm1 != MPI_COMM_NULL) {
        MPI_Comm_rank(newComm1, &newRank);
        for (int i = 0; i < size; ++i) {
            a[i] = 1;
        }
        MPI_Gather(&a, size, MPI_INT, a_received, size, MPI_INT, 0, newComm1);
    }

    if (newComm2 != MPI_COMM_NULL) {
        MPI_Comm_rank(newComm2, &newRank);
        for (int i = 0; i < size; ++i) {
            a[i] = 2;
        }
        MPI_Gather(&a, size, MPI_INT, a_received, size, MPI_INT, 0, newComm2);
    }

    if (newComm3 != MPI_COMM_NULL) {
        MPI_Comm_rank(newComm3, &newRank);
        for (int i = 0; i < size; ++i) {
            a[i] = 3;
        }
        MPI_Gather(&a, size, MPI_INT, a_received, size, MPI_INT, 0, newComm3);
    }

    if (newComm4 != MPI_COMM_NULL) {
        MPI_Comm_rank(newComm4, &newRank);
        for (int i = 0; i < size; ++i) {
            a[i] = 4;
        }
        MPI_Gather(&a, size, MPI_INT, a_received, size, MPI_INT, 0, newComm4);
    }

    if (newRank == 0 && newComm1 != MPI_COMM_NULL) {
        printf("Thread 0 from NEW_COMM_1 received a[i] = ");
        for (int i = 0; i < size * N; ++i) {
            printf("%d ", a_received[i]);
        }
        printf("\n");
    }

    if (newRank == 0 && newComm2 != MPI_COMM_NULL) {
        printf("Thread 0 from NEW_COMM_2 received a[i] = ");
        for (int i = 0; i < size * N; ++i) {
            printf("%d ", a_received[i]);
        }
        printf("\n");
    }

    if (newRank == 0 && newComm3 != MPI_COMM_NULL) {
        printf("Thread 0 from NEW_COMM_3 received a[i] = ");
        for (int i = 0; i < size * N; ++i) {
            printf("%d ", a_received[i]);
        }
        printf("\n");
    }

    if (newRank == 0 && newComm4 != MPI_COMM_NULL) {
        printf("Thread 0 from NEW_COMM_4 received a[i] = ");
        for (int i = 0; i < size * N; ++i) {
            printf("%d ", a_received[i]);
        }
        printf("\n");
    }

    MPI_Comm interComm12, interComm21, interComm34, interComm43;
    if (newComm1 != MPI_COMM_NULL) MPI_Intercomm_create(newComm1, 0, MPI_COMM_WORLD, 3, 0, &interComm12);
    if (newComm2 != MPI_COMM_NULL) MPI_Intercomm_create(newComm2, 0, MPI_COMM_WORLD, 0, 0, &interComm21);
    if (newComm3 != MPI_COMM_NULL) MPI_Intercomm_create(newComm3, 0, MPI_COMM_WORLD, 9, 1, &interComm34);
    if (newComm4 != MPI_COMM_NULL) MPI_Intercomm_create(newComm4, 0, MPI_COMM_WORLD, 6, 1, &interComm43);

    int interCommArray[size * N];

    if (newComm1 != MPI_COMM_NULL && newRank == 0){
        MPI_Status status;
        MPI_Send(a_received, size * N, MPI_INT, 0, 0, interComm12);
        MPI_Recv(interCommArray, size * N, MPI_INT, 0, 0, interComm12, &status);
        printf("Thread from NEW_COMM_1 received from thread from NEW_COMM_2 through interComm12 a[i] = ");
        for (int i = 0; i < size * N; ++i) {
            printf("%d ", interCommArray[i]);
        }
        printf("\n");
    }

    if (newComm2 != MPI_COMM_NULL && newRank == 0){
        MPI_Status status;
        MPI_Send(a_received, size * N, MPI_INT, 0, 0, interComm21);
        MPI_Recv(interCommArray, size * N, MPI_INT, 0, 0, interComm21, &status);
        printf("Thread from NEW_COMM_2 received from thread from NEW_COMM_1 through interComm21 a[i] = ");
        for (int i = 0; i < size * N; ++i) {
            printf("%d ", interCommArray[i]);
        }
        printf("\n");
    }

    if (newComm3 != MPI_COMM_NULL && newRank == 0){
        MPI_Status status;
        MPI_Send(a_received, size * N, MPI_INT, 0, 1, interComm34);
        MPI_Recv(interCommArray, size * N, MPI_INT, 0, 1, interComm34, &status);
        printf("Thread from NEW_COMM_3 received from thread from NEW_COMM_4 through interComm34 a[i] = ");
        for (int i = 0; i < size * N; ++i) {
            printf("%d ", interCommArray[i]);
        }
        printf("\n");
    }

    if (newComm4 != MPI_COMM_NULL && newRank == 0){
        MPI_Status status;
        MPI_Send(a_received, size * N, MPI_INT, 0, 1, interComm43);
        MPI_Recv(interCommArray, size * N, MPI_INT, 0, 1, interComm43, &status);
        printf("Thread from NEW_COMM_4 received from thread from NEW_COMM_3 through interComm43 a[i] = ");
        for (int i = 0; i < size * N; ++i) {
            printf("%d ", interCommArray[i]);
        }
        printf("\n");
    }


    if (newComm1 != MPI_COMM_NULL) {
        MPI_Comm_free(&interComm12);
        MPI_Comm_free(&newComm1);
    }

    if (newComm2 != MPI_COMM_NULL) {
        MPI_Comm_free(&interComm21);
        MPI_Comm_free(&newComm2);
    }

    if (newComm3 != MPI_COMM_NULL) {
        MPI_Comm_free(&interComm34);
        MPI_Comm_free(&newComm3);
    }

    if (newComm4 != MPI_COMM_NULL) {
        MPI_Comm_free(&interComm43);
        MPI_Comm_free(&newComm4);
    }

    MPI_Group_free(&newGroup1);
    MPI_Group_free(&newGroup2);
    MPI_Group_free(&newGroup3);
    MPI_Group_free(&newGroup4);
    MPI_Finalize();
    return 0;
}