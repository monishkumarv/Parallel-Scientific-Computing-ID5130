/* a simple Broadcast program in MPI */
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char** argv)
{
  int i, myid, size;
  int buf = 0;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size); /* number of processes in the communicator */
  MPI_Comm_rank(MPI_COMM_WORLD, &myid); /* fill the rank of each of the processes */
  
  if (myid == 0)
    buf = 327;

  /* collective communication function, that is called on all processes */
  MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (myid == 0) 
    printf("\n Broadcasted values on processors are:\n");
  
  printf("\t (%d, %d)\n", myid, buf); /* printed by all the processes */
  
  MPI_Finalize();

}
