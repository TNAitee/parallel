
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Run with -np 2.\n");

        MPI_Finalize();
        return 1;
    }

    int A[4] = {1, 2, 3, 4};
    int B[4] = {5, 6, 7, 8};
    int row[2], c_row[2];

    if (rank == 0) {
        row[0] = A[0];
        row[1] = A[1];

        MPI_Send(&A[2], 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(row, 2, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    c_row[0] = row[0] * B[0] + row[1] * B[2];
    c_row[1] = row[0] * B[1] + row[1] * B[3];

    if (rank == 0) {
        int C[4] = {c_row[0], c_row[1], 0, 0};

        MPI_Recv(&C[2], 2, MPI_INT, 1, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Matrix C = A x B\n");
        printf("%d %d\n%d %d\n", C[0], C[1], C[2], C[3]);

        printf("\nC[0][0] = 1x5 + 2x7 = %d\n", C[0]);
        printf("C[0][1] = 1x6 + 2x8 = %d\n", C[1]);
        printf("C[1][0] = 3x5 + 4x7 = %d\n", C[2]);
        printf("C[1][1] = 3x6 + 4x8 = %d\n", C[3]);
    } else {
        MPI_Send(c_row, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
