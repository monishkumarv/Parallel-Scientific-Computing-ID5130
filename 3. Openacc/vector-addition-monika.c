/*Simple vector addition using kernal directives*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void vecaddgpu(float *restrict r, float *a, float *b, int n)
{

#pragma acc kernels loop copyin(a [0:n], b [0:n]) copyout(r [0:n])

       for (int i = 0; i < n; ++i)
              r[i] = a[i] + b[i];
}

int main()
{
       int n;    /* vector length */
       float *a; /* input vector 1 */
       float *b; /* input vector 2 */
       float *r; /* output vector */
       float *e; /* expected output values */
       struct timespec ts1, ts2;
       
       int i, errs;
       n = 100000;
       a = (float *)malloc(n * sizeof(float));
       b = (float *)malloc(n * sizeof(float));
       r = (float *)malloc(n * sizeof(float));
       e = (float *)malloc(n * sizeof(float));
       
       for (i = 0; i < n; ++i)
       {
              a[i] = (float)(i + 1);
              b[i] = (float)(1000 * i);
       }

       clock_gettime(CLOCK_REALTIME, &ts1);
       /* compute on the GPU */
       vecaddgpu(r, a, b, n);
       /* compute on the host to compare */
       clock_gettime(CLOCK_REALTIME, &ts2);
       
       if (ts2.tv_nsec < ts1.tv_nsec)
       {
              ts2.tv_nsec += 1000000000;
              ts2.tv_sec--;
       }

       for (i = 0; i < n; ++i)
              e[i] = a[i] + b[i];
       /* compare results */
       
       errs = 0;
       for (i = 0; i < n; ++i)
       {
              if (r[i] != e[i])
              {
                     ++errs;
              }
       }

       printf("Time taken : %ld.%09ld\n", (long)(ts2.tv_sec - ts1.tv_sec),
              ts2.tv_nsec - ts1.tv_nsec);
       printf("%d errors found\n", errs);
       return errs;
}
