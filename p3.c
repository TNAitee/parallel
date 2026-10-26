
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, value = 100, received;
    double start, end, elapsed, max_elapsed;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    MPI_Sendrecv(&value, 1, MPI_INT, next, 0,
                 &received, 1, MPI_INT, prev, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == i) {
            if (rank == 0)
                printf("P0 sends 100 to P%d\n", next);

            printf("P%d receives %d from P%d\n",
                   rank, received, prev);

            if (rank != 0)
                printf("P%d sends 100 to P%d\n", rank, next);

            fflush(stdout);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("P0 sends 100 to P1\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        printf("Ring communication completed.\n");

    int sizes[] = {1, 100, 1000, 10000};
    char sendbuf[10000], recvbuf[10000];

    for (int i = 0; i < 4; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();

        MPI_Sendrecv(sendbuf, sizes[i], MPI_CHAR, next, 1,
                     recvbuf, sizes[i], MPI_CHAR, prev, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        end = MPI_Wtime();
        elapsed = end - start;

        MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE,
                   MPI_MAX, 0, MPI_COMM_WORLD);

        if (rank == 0)
            printf("Size = %d bytes, Time = %.9f seconds\n",
                   sizes[i], max_elapsed);
    }

    MPI_Finalize();
    return 0;
}
