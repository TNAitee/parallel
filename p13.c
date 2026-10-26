
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0)
            printf("Run with -np 3\n");

        MPI_Finalize();
        return 1;
    }

    int A[15] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15
    };

    int counts[3] = {6, 6, 3};
    int displs[3] = {0, 6, 12};
    int row_counts[3] = {2, 2, 1};

    int local_n = row_counts[rank];
    int local[6];

    // Distribute matrix rows
    MPI_Scatterv(
        A, counts, displs, MPI_INT,
        local, local_n * 3, MPI_INT,
        0, MPI_COMM_WORLD
    );

    // Multiply each element by 2
    for (int i = 0; i < local_n * 3; i++) {
        local[i] *= 2;
    }

    // Print each process's result
    printf("P%d:\n", rank);

    for (int i = 0; i < local_n; i++) {
        printf("%d %d %d\n",
               local[i * 3],
               local[i * 3 + 1],
               local[i * 3 + 2]);
    }

    // Collect processed rows
    int result[15];

    MPI_Gatherv(
        local, local_n * 3, MPI_INT,
        result, counts, displs, MPI_INT,
        0, MPI_COMM_WORLD
    );

    // Print final matrix
    if (rank == 0) {
        printf("Final matrix:\n");

        for (int i = 0; i < 5; i++) {
            printf("%d %d %d\n",
                   result[i * 3],
                   result[i * 3 + 1],
                   result[i * 3 + 2]);
        }
    }

    MPI_Finalize();
    return 0;
}
