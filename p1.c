/* p1.c: N must be divisible by the number of processes. */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char **argv) {
    int rank, size, N = 12;
    int data[12] = {10,20,30,40,50,60,70,80,90,100,110,120};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4 || N % size != 0) {
        if (rank == 0) fprintf(stderr, "Run with 4 processes for the sample.\n");
        MPI_Finalize(); return 1;
    }
    int count = N / size, local[3];
    MPI_Scatter(data, count, MPI_INT, local, count, MPI_INT, 0, MPI_COMM_WORLD);
 
    int left_value = -1, right_value = -1;
    MPI_Status status;
    /* Exchange last element to the right, and receive left neighbor's last element. */
    if (rank < size - 1)
        MPI_Send(&local[count-1], 1, MPI_INT, rank+1, 10, MPI_COMM_WORLD);
    if (rank > 0)
        MPI_Recv(&left_value, 1, MPI_INT, rank-1, 10, MPI_COMM_WORLD, &status);
 
    if (rank < size - 1)
        printf("P%d: Right boundary sent = %d\n", rank, local[count-1]);
    if (rank > 0)
        printf("P%d: Left boundary received = %d\n", rank, left_value);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) printf("Boundary exchange completed successfully.\n");
    MPI_Finalize();
    return 0;
}