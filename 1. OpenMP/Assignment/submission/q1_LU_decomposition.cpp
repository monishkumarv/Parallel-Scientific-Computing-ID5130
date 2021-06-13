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

double kernel()
{

    int n = 25;
    float a[n], b[n], c[n];

    float xl = 0, xu = 3, x;
    float f[n], h = (xu - xl) / (n - 1);

    for (int i = 0; i < n; i++)
    {

        x = xl + (i * h);
        f[i] = sin(5 * x);

        if (i == 0)
        {
            a[i] = 0;
            b[i] = 1;
            c[i] = 2;
        }
        else if (i == n - 1)
        {
            a[i] = 2;
            b[i] = 1;
            c[i] = 0;
        }
        else
        {
            a[i] = 1;
            b[i] = 4;
            c[i] = 1;
        }
    }

    float y[n];
    y[0] = (1 / h) * (-2.5 * f[0] + 2 * f[1] + 0.5 * f[2]);
    y[n - 1] = (1 / h) * (2.5 * f[n - 1] - 2 * f[n - 2] - 0.5 * f[n - 3]);

    for (int i = 1; i < n - 1; i++)
    {
        y[i] = (3 / h) * (f[i + 1] - f[i - 1]);
    }

    // LU Decompsition:
    double start = clock_time();

    float L[n], U[n];
    U[0] = b[0];
    L[0] = 0;
    for (int k = 0; k < n; k++)
    {
        L[k + 1] = a[k + 1] / U[k];
        U[k + 1] = b[k + 1] - L[k + 1] * c[k];
    }

    // Forward substituion:
    float z[n];
    z[0] = y[0];
    for (int i = 1; i < n; i++)
    {
        z[i] = y[i] - L[i] * z[i - 1];
    }

    // Backward substituion:
    float f_dash[n];
    for (int i = n - 1; i >= 0; i--)
    {
        f_dash[i] = (1 / U[i]) * (z[i] - c[i] * f_dash[i + 1]);
    }

    double end = clock_time();

    // Display output:
    std::ofstream myfile;
    myfile.open("q1_LU_decomposition.csv");
    myfile << "x,Numerical solution,Analytical solution\n";

    for (int i = 0; i < n; i++)
    {
        x = xl + (i * h);
        cout << "Numerical solution: " << f_dash[i] << " Analytical solution: " << 5 * cos(5 * x) << endl;
        myfile << x << "," << f_dash[i] << "," << 5 * cos(5 * x) << endl;
    }
    myfile.close();

    return (end - start);
}

int main()
{
    double time = kernel();
    printf("\nTotal time taken  = %fs\n", time);
    return 0;
}
