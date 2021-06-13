
// Serial version of trapezoidal rule for integration

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>

#define PI 3.14159265358

double func(double x)
{
  return (1.0 + sin(x));
}

void trapezoidal_rule(int n, double a, double b, double *result)
{
  double h, x, total;
  int i;

  h = (b-a)/n;		                        	// interval (or) width of the trapeziams

  total = (func(a) + func(b))/2.0;

  # pragma omp parallel for num_threads(5) reduction(+: total)
  for(i = 1; i <= n-1; i++) 	              // 'i' will be private for each 'thread'..
  {
    int q = omp_get_thread_num();
    printf(q);
    total += func(a + i*h);
  }
  
  total = total * h;
  *result = total; 
}


int main(void)
{
  double a, b, final_result;
  int n;

  n = 100;		                      // number of trapeziams
  a = 0.0;	                    		// left boundary
  b = PI;		                      	// right boundary (upper limit)
  final_result = 0.0;

  trapezoidal_rule(n, a, b, &final_result);

  printf("\n Converting Serial code -> Parallel form my just using '# pragma omp parallel for num_threads(5)' command... ");
  printf("\n     The area under the curve f(x) = 1+sin(x) between 0 to PI is equal to %lf \n\n", final_result);

  return 0;  
}
