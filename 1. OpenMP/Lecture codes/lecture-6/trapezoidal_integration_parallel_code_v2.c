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

double trapezoidal_rule(int n, double a, double b)
{
  double h, x, total; 		// will be local or PRIVATE to each thread..
  int i;
  int my_rank = omp_get_thread_num();
  int thread_count = omp_get_num_threads();
  int local_n;			                                        // local number of trapeziams to be integrated by each thread...
  double local_a, local_b;

  h = (b-a)/n;			                                          // h remains the same whether u are in serial or parallel

  local_n = n/thread_count;	                                  // local values for each thread...
  local_a = a + my_rank*local_n*h;
  local_b = local_a + local_n*h;
  
  total = (func(local_a) + func(local_b))/2.0;
  for(i = 1; i <= local_n-1; i++)                             // sharing the work, use only local_n
    {
      x = local_a + i*h;
      total += func(x);
    }
  total = total * h;

  return total;	                                          		/* total for each thread, private */
}


int main(int argc, char* argv[])
{
  double a, b, final_result;
  int n;
  int thread_count = 1;

  if (argc == 2)
    {
      thread_count = strtol(argv[1], NULL, 10);
    }
  else
    {
      printf("\n A command line argument other than name of the executable is required...exiting the program..");
      return 1;
    }

  n = 100;			                                          // number of trapeziums..
  a = 0.0;
  b = PI;


// Method 1: works like a serial code...not parallel...sad life :(
  
  final_result = 0.0;
# pragma omp parallel num_threads(thread_count)
  {
    # pragma omp critical                                 // since it is enclosed by '# pragma omp critical'
      final_result += trapezoidal_rule(n, a, b);          // it will get executed only one after the other...
  }



// Method 2: works like a parallel code as expected...happy!

  final_result = 0.0;
# pragma omp parallel num_threads(thread_count)
  {
    double my_result = 0.0;                               // my_result is local, PRIVATE for each thread !!
    my_result = trapezoidal_rule(n, a, b);                /* parallel call here as well */
    
    # pragma omp critical
      final_result += my_result; 
  }



// Method 3: same as 'Method:2', but much cleaner code...(using reduction clasue)..
  final_result = 0.0;
# pragma omp parallel num_threads(thread_count) reduction(+: final_result)     // call to trapezoidal rule will be parallel..
    final_result += trapezoidal_rule(n, a, b);                                 // ...while '+' operator will be sequential !!
  
  printf("\n The area under the curve f(x) = 1+sin(x) between 0 to PI is equal to %lf \n\n", final_result);

  return 0;  
}
