#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
#include <omp.h>
#include <sys/time.h>

#define print_array(a, n)             \
    cout << "Array '" << #a << "': "; \
    for (int i = 0; i < n; i++)       \
        cout << a[i] << " ";          \
    cout << endl;

double clock_time()
{
    struct timeval Tp;
    int stat;
    stat = gettimeofday(&Tp, NULL);
    if (stat != 0)
        printf("Error return from gettimeofday: %d", stat);
    return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

double q(double x, double y)
{
    return (2 * (2 - pow(x, 2) - pow(y, 2)));
}

double phi(double x, double y)
{
    return ((pow(x, 2) - 1) * (pow(y, 2) - 1));
}

void kernel()
{
    double delta = 0.1;

    int n = (2 / delta) - 1;
    double a[n * n][n * n] = {0}, b[n * n], x[n * n];

    for (int k = 0; k < n * n; k++)
    {
        a[k][k] = -4;
        if (k - 1 >= 0)
            a[k][k - 1] = 1;
        if (k + 1 < n * n)
            a[k][k + 1] = 1;
        if (k - n >= 0)
            a[k][k - n] = 1;
        if (k + n < n * n)
            a[k][k + n] = 1;
    }

    double ii, jj;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            ii = -1 + (i + 1) * delta;
            jj = -1 + (j + 1) * delta;

            b[i] = pow(delta, 2) * q(ii, jj);

            if (i == 0 || i == n - 1 || j == 1 || j == n - 1)
            {
                b[i] = b[i] - 0;
            }
        }
    }

    // Gauss-Seidel method:

    double sum, error = 1;
    int it = 100;
    while (it--)
    {
        for (int i = 0; i < n * n; i++)
        {
            sum = 0.0;
            for (int j = 0; j < n * n; j++)
            {
                if (i != j)
                {
                    sum = sum + a[i][j] * x[j];
                }
            }

            x[i] = (1 / a[i][i]) * (b[i] - sum);
        }
    }

    print_array(x,n);
}

int main()
{
    double start = 0.0, end = 0.0;
    start = clock_time();
    kernel();
    end = clock_time();
    printf("Total time taken  = %fs\n", end - start);
    return 0;
}
