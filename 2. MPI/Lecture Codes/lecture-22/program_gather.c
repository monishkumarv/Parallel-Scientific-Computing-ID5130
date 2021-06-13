#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char** argv)
{
  int i, myid, size;
  int send_buf, *recv_buf; 
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  /* allocate memory for recv_buf on host process: 0  */
  if (myid == 0) 
    {
      recv_buf = (int *)malloc(size*sizeof(int));
    }

  /* different processes  */
  send_buf = 100+myid*myid; 	/* single integer value */

  /* collective communication called by all the processes */
  MPI_Gather(&send_buf, 1, MPI_INT, recv_buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  if (myid == 0) 
    {
      printf("\n Received values on host process are:\n");
      for(i=0; i<size; i++)
	      printf("\t %d", recv_buf[i]);
      printf("\n");
    }

  if (myid == 0)
    free(recv_buf);

  MPI_Finalize();
  
}
