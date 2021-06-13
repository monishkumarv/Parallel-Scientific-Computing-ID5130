#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

float dot_product(float a[], float b[], int n)
{
  int i; 
  float sum = 0.0;

  for (i = 0; i < n; i++)
    sum = sum + a[i]*b[i];
  
  return sum;
}

int main(int argc, char** argv)
{
  float *vector1, *vector2;
  int i, n = 5;
  float local_dot, global_dot;
  /* parallel variables */
  int myid, size;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  /* allocate memory on all processes */
  vector1 = (float*)malloc(n*sizeof(float));
  vector2 = (float*)malloc(n*sizeof(float));

  /* fill values */
  for (i = 0; i < n; i++)
    {
      vector1[i] = i+1+myid;
      vector2[i] = i+2+myid;
    }

  /* calculated on each process */
  local_dot = dot_product(vector1, vector2, n);

  MPI_Allreduce(&local_dot, &global_dot, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

  /* printf("\n Vector 1 is:"); */
  /* for(i=0; i<n; i++) */
  /*   printf("\t %f", vector1[i]); */

  /* printf("\n Vector 2 is:"); */
  /* for(i=0; i<n; i++) */
  /*   printf("\t %f", vector2[i]); */

  /* if (myid == 0) */
  printf("\n Inner product of V1 and V2 is = %f \n", global_dot);

  free(vector1);
  free(vector2);

  MPI_Finalize();
}
