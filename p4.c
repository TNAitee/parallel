#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int rank, size, iters = 5;
    int bytes = 1024;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        MPI_Finalize();
        return 1;
    }

     if (argc > 1)
        bytes = atoi(argv[1]);

    char *buf = malloc(bytes);

    if (buf == NULL) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (int i = 0; i < bytes; i++)
        buf[i] = 'A';

    MPI_Barrier(MPI_COMM_WORLD);

        double start = MPI_Wtime();

    for (int i = 0; i < iters; i++) {
        if (rank == 0) {
            MPI_Send(buf, bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buf, bytes, MPI_BYTE, 1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(buf, bytes, MPI_BYTE, 0, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buf, bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        }
    }

    double elapsed = MPI_Wtime() - start;

    if (rank == 0) {
        double rt = elapsed / iters;

        printf("Message size          : %d bytes\n", bytes);
        printf("Iterations             : %d\n", iters);
        printf("Total round trips      : %d\n", iters);
        printf("Average round-trip     : %.6f seconds\n", rt);
        printf("Average one-way        : %.6f seconds\n", rt / 2.0);
    }

    free(buf);
    MPI_Finalize();
    return 0;
}