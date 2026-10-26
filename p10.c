#include <mpi.h>
#include <stdio.h>
 
int main(int argc, char **argv) {
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(size!=3) { if(rank==0) fprintf(stderr,"Run with -np 3.\n"); MPI_Finalize(); return 1; }
    const int items[8]={10,20,30,40,50,60,70,80};
    if(rank==0) {
        for(int i=0;i<8;++i) {
            int dest=1+(i%2);
            MPI_Send(&items[i],1,MPI_INT,dest,1,MPI_COMM_WORLD);
            printf("P0 produced: %d (sent to P%d)\n",items[i],dest);
        }
        int stop=-1;
        MPI_Send(&stop,1,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Send(&stop,1,MPI_INT,2,0,MPI_COMM_WORLD);
    } else {
        int value;
        while(1) {
            MPI_Recv(&value,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            if(value==-1) break;
            printf("P%d consumed: %d\n",rank,value);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0) printf("All 8 items processed successfully.\n");
    MPI_Finalize(); return 0;
}