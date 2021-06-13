/* OpenMP program to demonstrate scope of private variables */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#ifdef _OPENMP
#include<omp.h>
#endif

int main(int argc, char* argv[])
{
  int x, thread_count = 1;

  if (argc == 2)
    {
      thread_count = strtol(argv[1], NULL, 10);
    }
  else
    {
      printf("\n A command line argument other than name of the executable is required...exiting the program..");
      return 1;
    }


  // x initialized it here...
  x = -99;
# pragma omp parallel num_threads(thread_count) default(none) private(x)
  {
    int myrank;
    #ifdef _OPENMP    
        myrank = omp_get_thread_num();
    #else
        myrank = 0;
    #endif

    // value of x is random...(coz values of 'private' variable is unspecified)
    printf("\n Value of x is %d on thread id %d before initialization", x, myrank);

    x = myrank*10;
    
    // value of x is now = myrank * 10
    printf("\n Value of x is %d on thread id %d after initialization", x, myrank);

  }
  
  // value of x is now = -99
  printf("\n Value of x is %d after exit from parallel for ", x);
}
