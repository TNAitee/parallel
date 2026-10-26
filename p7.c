
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, a[8], b[8];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int temp[8] = {10, 20, 30, 40, 50, 60, 70, 80};
        for (int i = 0; i < 8; i++)
            a[i] = temp[i];
    } else {
        int temp[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        for (int i = 0; i < 8; i++)
            a[i] = temp[i];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    MPI_Sendrecv(a, 8, MPI_INT, 1 - rank, 0,
                 b, 8, MPI_INT, 1 - rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    double blocking_time = MPI_Wtime() - start;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Request req1, req2;

    MPI_Irecv(b, 8, MPI_INT, 1 - rank, 1,
              MPI_COMM_WORLD, &req1);

    MPI_Isend(a, 8, MPI_INT, 1 - rank, 1,
              MPI_COMM_WORLD, &req2);

    MPI_Wait(&req1, MPI_STATUS_IGNORE);
    MPI_Wait(&req2, MPI_STATUS_IGNORE);

    double nonblocking_time = MPI_Wtime() - start;

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("P0 received: ");
        for (int i = 0; i < 8; i++)
            printf("%d ", b[i]);
        printf("\n");
    } else {
        printf("P1 received: ");
        for (int i = 0; i < 8; i++)
            printf("%d ", b[i]);
        printf("\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Nonblocking communication completed.\n");
        printf("Blocking communication time    = %.6f s\n",
               blocking_time);
        printf("Nonblocking communication time = %.6f s\n",
               nonblocking_time);
    }

    MPI_Finalize();
    return 0;
}
