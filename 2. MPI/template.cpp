#include <bits/stdc++.h>
#include <stdio.h>
#include <mpi.h>

using namespace std;
#include <sys/time.h>

double kernel(int myid, int size, MPI_Status status)
{

  // Code goes here..

  double end = MPI_Wtime();
  return end;
}

int main(int argc, char **argv)
{
  MPI_Init(NULL, NULL);
  double start = MPI_Wtime();

  int myid, size;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  double end = kernel(myid, size, status);
  if (myid == 0)
    printf("\nTotal time taken  = %fs\n", (end - start));

  MPI_Finalize();

  return 0;
}