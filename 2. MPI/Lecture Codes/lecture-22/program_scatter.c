#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char** argv)
{
  int i, myid, size;
  int *send_buf, recv_buf; 
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  /* allocate memory for send_buf on the host process process - 0 */
  if (myid == 0) 
    {
      send_buf = (int *)malloc(size*sizeof(int)); /* array of length size (nprocs) */
      for(i=0; i<size; i++)
	      send_buf[i] = 100+i*5+i;
    }

  /* collective communication, all processes will call this function  */
  MPI_Scatter(send_buf, 1, MPI_INT, &recv_buf, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (myid == 0) 
    printf("\n Received values on processors are:\n");

  /* all the processes will print this message */
  printf("\t (%d, %d)", myid, recv_buf);

  if (myid == 0)
    free(send_buf);

  MPI_Finalize();
  
}
