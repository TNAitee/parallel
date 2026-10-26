#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int a[2], all[8];
    int i, j, temp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("Run with 4 processes.\n");

        MPI_Finalize();
        return 0;
    }

    int data[8] = {45, 12, 78, 3, 56, 23, 89, 10};

    a[0] = data[rank * 2];
    a[1] = data[rank * 2 + 1];

    /* Sort local elements */
    if (a[0] > a[1]) {
        temp = a[0];
        a[0] = a[1];
        a[1] = temp;
    }

    printf("P%d: %d %d\n", rank, a[0], a[1]);

    /* Gather local arrays */
    MPI_Gather(a, 2, MPI_INT, all, 2, MPI_INT,
               0, MPI_COMM_WORLD);

    /* Sort the complete array on P0 */
    if (rank == 0) {
        for (i = 0; i < 8; i++) {
            for (j = i + 1; j < 8; j++) {
                if (all[i] > all[j]) {
                    temp = all[i];
                    all[i] = all[j];
                    all[j] = temp;
                }
            }
        }

        printf("Sorted array: ");

        for (i = 0; i < 8; i++) {
            printf("%d ", all[i]);
        }

        printf("\n");
    }

    MPI_Finalize();
    return 0;
}