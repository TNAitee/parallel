
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size, sendval, recvval;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Run with -np 2.\n");
        MPI_Finalize();
        return 1;
    }

    sendval = (rank == 0) ? 100 : 200;

    if (rank == 0) {
        printf("Number of processes: 2\n\n");
        printf("P0 message = 100\n\n");
        printf("P1 message = 200\n\n");
        printf("Unsafe version:\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        printf("P0: Waiting to send 100\n");
    else
        printf("P1: Waiting to send 200\n");

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        printf("Potential deadlock detected.\n\n");
    
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        printf("Corrected version:\n");

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Sendrecv(&sendval, 1, MPI_INT, 1-rank, 0,
                 &recvval, 1, MPI_INT, 1-rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (rank == 0) {
        printf("P0 sent %d\n", sendval);
        printf("P0 received %d\n", recvval);
    } else {
        printf("P1 received %d\n", recvval);
        printf("P1 sent %d\n", sendval);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        printf("Communication completed without deadlock.\n");

    MPI_Finalize();
    return 0;
}
