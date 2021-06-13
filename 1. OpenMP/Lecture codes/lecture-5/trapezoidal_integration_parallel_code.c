
// OpenMP parallel version of trapezoidal rule

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#ifdef _OPENMP
  #include<omp.h>
#endif

#define PI 3.14159265358

double func(double x)
{
  return (1.0 + sin(x));
}

void trapezoidal_rule(int n, double a, double b, double *result)
{
  double h, x, total; 		                              // will be local or PRIVATE to each thread...
  int i;

  #ifdef _OPENMP
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
  #else
    int my_rank = 0;
    int thread_count = 1;
  #endif

  int local_n;			                                    // local number of trapeziams to be integrated by each thread...
  double local_a, local_b;

  h = (b-a)/n;			                                    // h remains the same whether u are in serial or parallel

  local_n = n/thread_count;	                            // local values for each thread...
  local_a = a + my_rank*local_n*h;
  local_b = local_a + local_n*h;
  
  total = (func(local_a) + func(local_b))/2.0;
  for(i = 1; i <= local_n-1; i++) {                     // sharing the work, use only local_n
      x = local_a + i*h;
      total += func(x);
    }
  total = total * h;

#pragma omp critical                      // makes all threads to update result in a 'queue' *********************
  *result += total; 		                  // race condition... (all threads may compete to update 'result')
                                          // you will get a wrong answer if you dont use '#pragma omp critical' !!
}


int main(int argc, char* argv[])
{
  double a, b, final_result;
  int n;
  int thread_count = 1;

  if (argc == 2) {
      thread_count = strtol(argv[1], NULL, 10);
    }
  else {
      printf("\n A command line argument other than name of the executable is required...exiting the program..");
      return 1;
    }

  n = 100;			                          // number of trapeziums..
  a = 0.0;
  b = PI;
  final_result = 0.0;

  #ifdef _OPENMP
    printf("\nParallel Code:");
  #else
    printf("\nSerial Code:");
  #endif

# pragma omp parallel num_threads(thread_count)
  trapezoidal_rule(n, a, b, &final_result);

  printf("\n The area under the curve f(x) = 1+sin(x) between 0 to PI is equal to %lf \n\n", final_result);

  return 0;  
}
