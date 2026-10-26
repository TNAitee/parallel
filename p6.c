#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int a[2], left_halo = 0, right_halo = 0;
    int result[2];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
t
    if (size != 4) {
        MPI_Finalize();
        return 1;
    }

    a[0] = rank * 20 + 10;
    a[1] = rank * 20 + 20;

    if (rank > 0) {
        MPI_Send(&a[0], 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&left_halo, 1, MPI_INT, rank - 1, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (rank < size - 1) {
        MPI_Send(&a[1], 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
        MPI_Recv(&right_halo, 1, MPI_INT, rank + 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int left = a[0];
    int right = a[1];

    if (rank > 0)
        left = left_halo;

    if (rank < size - 1)
        right = right_halo;

    result[0] = left + a[0] + a[1];
    result[1] = a[0] + a[1] + right;

    printf("P%d: %d %d | Result: %d %d\n",
           rank, a[0], a[1], result[0], result[1]);

    MPI_Finalize();
    return 0;
}