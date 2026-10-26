
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *sendbuf = malloc((size_t)size * sizeof(int));
    int *recvbuf = malloc((size_t)size * sizeof(int));

    if (sendbuf == NULL || recvbuf == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (int dest = 0; dest < size; dest++)
        sendbuf[dest] = rank * 10 + dest;

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    for (int k = 0; k < 1000; k++)
        MPI_Alltoall(sendbuf, 1, MPI_INT,
                     recvbuf, 1, MPI_INT, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - start;
    double max_elapsed;

    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    /* Print process results in rank order */
    for (int p = 0; p < size; p++) {
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == p) {
            printf("P%d received:", rank);
            for (int i = 0; i < size; i++)
                printf(" %d", recvbuf[i]);
            printf("\n");
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        printf("1000 MPI_Alltoall calls; maximum rank time = %.9f s\n",
               max_elapsed);

    free(sendbuf);
    free(recvbuf);

    MPI_Finalize();
    return 0;
}
