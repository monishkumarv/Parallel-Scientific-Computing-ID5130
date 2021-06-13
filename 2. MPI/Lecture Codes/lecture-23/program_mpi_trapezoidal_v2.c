/* MPI parallel version of trapezoidal rule that makes use of derived data types */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

#define PI 3.14159265358

double func(double x)
{
  return (1.0 + sin(x));
}

double trapezoidal_rule(double la, double lb, double ln, double h)
{
  double total;
  double x;
  int i;

  total = (func(la) + func(lb))/2.0;
  for(i = 1; i <= ln-1; i++) /* sharing the work, use only local_n */
    {
      x = la + i*h;
      total += func(x);
    }
  total = total * h;

  return total;			/* total for each thread, private */
}

void create_new_mpi_type(double* a_p, double* b_p, int* n_p, MPI_Datatype* new_mpi_type_p)
{
  int block_lengths[3] = {1, 1, 1};
  MPI_Datatype types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
  MPI_Aint a_address, b_address, n_address; /* Address int */
  MPI_Aint displacements[3] = {0, 0, 0};

  MPI_Get_address(a_p, &a_address); /* the addresses will be corresponding to each process.. */
  MPI_Get_address(b_p, &b_address);
  MPI_Get_address(n_p, &n_address);

  displacements[1] = b_address - a_address; /* offset wrt to the first argument */
  displacements[2] = n_address - a_address;

  /* create new MPI structure */
  MPI_Type_create_struct(3, block_lengths, displacements, types, new_mpi_type_p);

  /* commit the new MPI type  */
  MPI_Type_commit(new_mpi_type_p);
}

void read_user_input(int myid, int nprocs, double* a_p, double* b_p, int* n_p)
{
  MPI_Datatype new_mpi_type;

  create_new_mpi_type(a_p, b_p, n_p, &new_mpi_type);

  if (myid == 0)
    {
      printf("\n Enter values for a, b and n \n");
      scanf("%lf %lf %d", a_p, b_p, n_p);
    }

  /* Broadcast the values to all the processes */

  MPI_Bcast(a_p, 1, new_mpi_type, 0, MPI_COMM_WORLD);

  /* free the new MPI type */

  MPI_Type_free(&new_mpi_type); /* free any memory that MPI has allocated while creating a new derived data type */
}


int main(int argc, char* argv[])
{
  double a, b, final_result, la, lb, lsum, h;
  int myid, nprocs, proc;
  int n, ln;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);	  /* myrank of the process */
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs); /* size of the communicator */

  /* n = 1024;			/\* number of trapezoids.. *\/ */
  /* a = 0.0; */
  /* b = PI;			/\* hard-coded.. *\/ */

  read_user_input(myid, nprocs, &a, &b, &n);

  final_result = 0.0;

  h = (b-a)/n;
  ln = n/nprocs; 		/* nprocs evenly divides number of trapezoids */

  la = a + myid*ln*h;
  lb = la + ln*h;
  lsum = trapezoidal_rule(la, lb, ln, h); /* every process calls this function... */

  if (myid != 0)
    {
      MPI_Send(&lsum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
  else				/* process 0 */
    {
      final_result = lsum;
      for (proc = 1; proc < nprocs; proc++)
      {
        MPI_Recv(&lsum, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        final_result += lsum;
      }
    }

  if (myid == 0) 		/* output is only printed by process 0 */
    {
      printf("\n The area under the curve (1+sin(x)) between 0 to PI is equal to %lf \n\n", final_result);
    }
  
  MPI_Finalize();
  return 0;  
}
