#include <bits/stdc++.h>
#include <stdio.h>
#include <mpi.h>

using namespace std;
#include <sys/time.h>

double kernel(int myid, int size, MPI_Status status)
{

  // Initialize...
  long int n = 100000, l_n = n / size;

  float xl = 0, xu = 3, x, h;
  float f[n], f_dash[n];
  h = (xu - xl) / (n - 1);

  // Local upper & lower limits and local derivative...
  float l_xu, l_xl, local_f_dash[l_n];
  l_xl = xl + myid * l_n * h;
  l_xu = l_xl + (l_n - 1) * h;

  for (int i = 0; i < l_n; i++)
  {
    x = l_xl + (i * h);
    f[i] = sin(5 * x);
  }

  // Calculate derivative...

  local_f_dash[0] = (-f[2] + 8 * f[1] - 8 * sin(5 * (l_xl + (-1 * h))) + sin(5 * (l_xl + (-2 * h)))) / (12 * h);
  local_f_dash[1] = (-f[3] + 8 * f[2] - 8 * f[0] + sin(5 * (l_xl + (-1 * h)))) / (12 * h);

  for (int i = 2; i < l_n - 2; i++)
  {
    local_f_dash[i] = (-f[i + 2] + 8 * f[i + 1] - 8 * f[i - 1] + f[i - 2]) / (12 * h);
  }

  local_f_dash[l_n - 2] = (-sin(5 * (l_xu + (1 * h))) + 8 * f[l_n - 1] - 8 * f[l_n - 3] + f[l_n - 4]) / (12 * h);
  local_f_dash[l_n - 1] = (-sin(5 * (l_xu + (2 * h))) + 8 * sin(5 * (l_xu + (1 * h))) - 8 * f[l_n - 2] + f[l_n - 3]) / (12 * h);

  if (myid == 0)
  {
    local_f_dash[0] = (f[1] - f[0]) / h;
    local_f_dash[1] = (f[2] - f[1]) / h;
  }
  else if (myid == size - 1)
  {

    local_f_dash[l_n - 2] = (f[l_n - 2] - f[l_n - 3]) / h;
    local_f_dash[l_n - 1] = (f[l_n - 1] - f[l_n - 2]) / h;
  }

  // Gather all derivatives from all threads and display
  MPI_Gather(local_f_dash, l_n, MPI_INT, f_dash, l_n, MPI_INT, 0, MPI_COMM_WORLD);

  double end = MPI_Wtime();

  // if (myid == 0)
  // {
  //   // Display output:
  //   std::ofstream myfile;
  //   myfile.open("q1_b_parallel_code.csv");
  //   myfile << "x,Numerical solution,Analytical solution\n";

  //   for (int i = 0; i < n; i++)
  //   {
  //     x = xl + (i * h);
  //     cout << "Numerical solution: " << f_dash[i] << " Analytical solution: " << 5 * cos(5 * x) << endl;
  //     myfile << x << "," << f_dash[i] << "," << 5 * cos(5 * x) << endl;
  //   }
  //   myfile.close();
  // }

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