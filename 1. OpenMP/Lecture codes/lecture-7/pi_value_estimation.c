// OpenMP program to highlight for loop exceptions
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#ifdef _OPENMP
#include<omp.h>
#endif

#define N 1000

int main(int argc, char* argv[])
{
  int k, thread_count = 1;
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



// Wrong method...
# pragma omp parallel for num_threads(thread_count) reduction(+: sum) 
  for (k = 0; k < N; k++)	                          // threads will work in parallel
    {
      sum += sign/(2*k+1);
      sign = -sign; 		                            // 'sign' is a shared variable among threads (depends on previous..)
    }
  
  pi_value = 4.0*sum;
  printf("\n The approximate value of PI using series expansion is %lf (will be wrong if thread count is high) \n", pi_value);


// Correct method...(using private clause..)
sum = 0.0;
# pragma omp parallel for num_threads(thread_count) reduction(+: sum) private(sign)
  for (k = 0; k < N; k++)	                   // threads will work in parallel 
    {
      if (k % 2 == 0)
	      sign = 1.0;                          // make sign 'private' to thread by using... private(<variable name>) clause..
      else
	      sign = -1.0;		
     
      sum += sign/(2*k+1);     
    }
  
  pi_value = 4.0 * sum;
  printf("\n The approximate value of PI using series expansion is %lf (correct answer) \n", pi_value);

}
