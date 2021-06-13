#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

double clock_time()
{
    struct timeval Tp;
    int stat;
    stat = gettimeofday(&Tp, NULL);
    if (stat != 0)
        printf("Error return from gettimeofday: %d", stat);
    return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

void kernel()
{
    static float b[200 + 2][800 + 2] = {0}, a[600 + 2][200 + 2] = {0}, c[600 + 2][800 + 2] = {0};

    #pragma omp parallel num_threads(8)
    {
        #pragma omp for
        for (int i = 0; i <= 600; i++)
        {
            for (int j = 0; j <= 800; j++)
            {
                for (int k = 0; k <= 200; k++)
                {
                    #pragma omp atomic
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }
}
int main()
{
    double start = 0.0, end = 0.0;
    start = omp_get_wtime();
    kernel();
    end = omp_get_wtime();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}