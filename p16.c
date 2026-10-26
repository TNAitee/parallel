
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size, local, prefix, manual;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local = (rank + 1) * 10;

    MPI_Scan(&local, &prefix, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0)
        manual = local;
    else {
        int prev_prefix;
        MPI_Recv(&prev_prefix, 1, MPI_INT, rank - 1, 5,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        manual = prev_prefix + local;
    }

    if (rank < size - 1)
        MPI_Send(&manual, 1, MPI_INT, rank + 1, 5, MPI_COMM_WORLD);

    printf("P%d: Local value = %d, MPI_Scan prefix = %d, manual prefix = %d\n",
           rank, local, prefix, manual);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        int results[4];
        results[0] = prefix;

        for (int i = 1; i < size; i++)
            MPI_Recv(&results[i], 1, MPI_INT, i, 10,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("\nPrefix result: ");
        for (int i = 0; i < size; i++)
            printf("%d ", results[i]);
        printf("\n");
    } else {
        MPI_Send(&prefix, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
