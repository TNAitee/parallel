#include <mpi.h>
#include <stdio.h>
 
int main(int argc, char **argv) {
    MPI_Init(&argc,&argv);
    int rank,size,local[10]={0},global[10];
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(size!=4) { if(rank==0) fprintf(stderr,"Run with -np 4.\n"); MPI_Finalize(); return 1; }
    int samples[4][4]={{1,2,2,3},{2,3,4,4},{1,4,5,5},{0,2,5,9}};
    for(int i=0;i<4;++i) local[samples[rank][i]]++;
    MPI_Allreduce(local,global,10,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    if(rank==0) {
        printf("Value  Frequency\n");
        for(int i=0;i<10;++i) printf("%d      %d\n",i,global[i]);
        printf("Total number of elements = 16\n");
    }
    MPI_Finalize(); return 0;
}