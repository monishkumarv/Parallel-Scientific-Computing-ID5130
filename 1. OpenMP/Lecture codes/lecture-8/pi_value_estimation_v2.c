// OpenMP program to highlight for loop exceptions
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#ifdef _OPENMP
#include<omp.h>
#endif


int main(int argc, char* argv[])
{
  int n = 1000, k, thread_count = 1;
  double sign = 1.0;
  double sum = 0.0;
  double pi_value = 0.0;	                          // approximate value to be calculated...

  if (argc == 2)
    {
      thread_count = strtol(argv[1], NULL, 10);
    }
  else
    {
      printf("\n A command line argument other than name of the executable is required...exiting the program..");
      return 1;
    }

sum = 0.0;
# pragma omp parallel for num_threads(thread_count) default(none) reduction(+: sum) private(sign, k) shared(n)
  for (k = 0; k < n; k++)
    {
      if (k % 2 == 0)
	      sign = 1.0;
      else
	      sign = -1.0;		
     
      sum += sign/(2*k+1);     
    }
  
  pi_value = 4.0 * sum;
  printf("\n The approximate value of PI using series expansion is %lf (correct answer) \n", pi_value);

}
