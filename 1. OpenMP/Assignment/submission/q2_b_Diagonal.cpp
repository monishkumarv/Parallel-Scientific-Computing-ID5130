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

double kernel()
{
    double delta = 0.005;
    int n = (2 / delta) + 1;

    double x[n][n];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            x[i][j] = 0.0;
        }
    }

    // Diagonal method:
    double start = clock_time();

    double ii, jj, error = 1;
    int istart, iend;
    int nd = 2 * (n - 2);
    int thread_count = 8;

    while (error > pow(10, -2))
    {
        error = 0.0;
        for (int l = 2; l <= nd; l++)
        {
            if (l <= n - 1)
            {
                istart = 1;
                iend = l - 1;
            }
            else
            {
                istart = l - (n - 2);
                iend = n - 2;
            }

#pragma omp parallel for num_threads(thread_count) private(ii, jj) reduction(max \
                                                                             : error)
            for (int i = istart; i <= iend; i++)
            {
                int j = l - i;
                ii = -1 + i * delta;
                jj = -1 + j * delta;

                double bij = pow(delta, 2) * q(ii, jj);

                x[i][j] = 0.25 * (x[i + 1][j] + x[i - 1][j] + x[i][j + 1] + x[i][j - 1] + bij);

                double err_ij = abs((phi(ii, jj) - x[i][j]) / phi(ii, jj));
                error = error > err_ij ? error : err_ij;
            }
        }
    }

    double end = clock_time();

    // Display output:
    std::ofstream myfile;
    myfile.open("q2_b_Diagonal.csv");
    myfile << "x,y,Analytical solution,Numerical solution\n";

    for (int i = 1; i < n - 1; i++)
    {
        for (int j = 1; j < n - 1; j++)
        {
            ii = -1 + i * delta;
            jj = -1 + j * delta;
            cout << phi(ii, jj) << " " << x[i][j] << endl;
            myfile << ii << "," << jj << "," << phi(ii, jj) << "," << x[i][j] << endl;
        }
    }

    cout << "\nDelta = " << delta;
    cout << "\nThreads (p) = " << thread_count;
    cout << "\nError: " << error << endl;

    return (end - start);
}

int main()
{
    double time = kernel();
    printf("\nTotal time taken  = %fs\n", time);
    return 0;
}
