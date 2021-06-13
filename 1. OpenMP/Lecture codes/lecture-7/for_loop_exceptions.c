//OpenMP program to highlight for loop exceptions
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#ifdef _OPENMP
#include<omp.h>
#endif

#define N 30

int main(int argc, char* argv[])
{
  int a[N];
  int i, thread_count = 1;

  if (argc == 2)
    {
      thread_count = strtol(argv[1], NULL, 10);
    }
  else
    {
      printf("\n A command line argument other than name of the executable is required...exiting the program..");
      return 1;
    }
  

// correct way: we can only use basic for loops
# pragma omp parallel for num_threads(thread_count)
  for(i = 0 ; i < N; i++ )	
  {
    a[i] = i+i^2;
  }

// Exception 1: below code will throw compilation error coz it is not a structured block
int key = 5 + 5^2; // search postion for a key (value)
#pragma omp parallel for num_threads(thread_count)
  for(i = 0; i < N; i++)                            // not a structured block of code...coz it has multiple exits!
    if (a[i] == key) break;	                        // you can use 'exit'..but not 'break' (or) 'return' statements
  
printf("\n Found the key at location, i = %d", i);


// // Exception 2: cannot use loops with infinite/unknown upperbounds (eg. while, do while...)
// # pragma omp parallel for num_threads(thread_count)
//   for( ; ; )	                                      // infinite for loop...
//   {
//     a[i] = i+i^2;
//   }
  
}
