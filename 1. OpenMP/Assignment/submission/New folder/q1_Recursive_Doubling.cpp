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

    int n = 1000;
    float a[n], b[n], c[n];
    float a_prev[n], b_prev[n], c_prev[n], y_prev[n];

    float xl = 0, xu = 3, x;
    float f[n], h = (xu - xl) / (n - 1);

    for (int i = 0; i < n; i++)
    {

        x = xl + (i * h);
        f[i] = sin(5 * x);

        if (i == 0)
        {
            a[i] = a_prev[i] = 0;
            b[i] = b_prev[i] = 1;
            c[i] = c_prev[i] = 2;
        }
        else if (i == n - 1)
        {
            a[i] = a_prev[i] = 2;
            b[i] = b_prev[i] = 1;
            c[i] = c_prev[i] = 0;
        }
        else
        {
            a[i] = a_prev[i] = 1;
            b[i] = b_prev[i] = 4;
            c[i] = c_prev[i] = 1;
        }
    }

    float y[n];
    y[0] = (1 / h) * (-2.5 * f[0] + 2 * f[1] + 0.5 * f[2]);
    y[n - 1] = (1 / h) * (2.5 * f[n - 1] - 2 * f[n - 2] - 0.5 * f[n - 3]);

    for (int i = 1; i < n - 1; i++)
    {
        y[i] = (3 / h) * (f[i + 1] - f[i - 1]);
    }

    // ....................... Recursive doubling ........................ //
    double start = clock_time();

    float f_dash[n];
    for (int k = 1; k <= ceil(log2(n)); k++)
    {

        for (int i = 0; i < n; i++)
        {
            a_prev[i] = a[i];
            b_prev[i] = b[i];
            c_prev[i] = c[i];
            y_prev[i] = y[i];
        }

#pragma omp parallel for num_threads(8)
        for (int i = 0; i < n; i++)
        {

            int q = i - pow(2, k - 1);
            int w = i + pow(2, k - 1);

            float alpha = 0, beta = 0;

            a[i] = c[i] = 0;

            if (i >= pow(2, k - 1))
            {
                alpha = -1 * (a_prev[i] / b_prev[q]);
            }

            if (i <= (n - pow(2, k - 1) - 1))
            {
                beta = -1 * (c_prev[i] / b_prev[w]);
            }

            if (i >= pow(2, k))
            {
                a[i] = alpha * a_prev[q];
            }

            if (i <= (n - pow(2, k) - 1))
            {
                c[i] = beta * c_prev[w];
            }

            b[i] = alpha * c_prev[q] + b_prev[i] + beta * a_prev[w];
            y[i] = alpha * y_prev[q] + y_prev[i] + beta * y_prev[w];
            f_dash[i] = y[i] / b[i];
        }
    }

    double end = clock_time();

    // Display output:
    std::ofstream myfile;
    myfile.open("q1_Recursive_Doubling.csv");

    myfile << "x,Numerical solution,Analytical solution\n";

    for (int i = 0; i < n; i++)
    {
        x = xl + (i * h);
        cout << "f_dash[" << i << "] = " << f_dash[i] << ", " << 5 * cos(5 * x) << endl;
        myfile << x << "," << f_dash[i] << "," << 5 * cos(5 * x) << endl;
    }

    return (end - start);
}

int main()
{
    double time = kernel();
    printf("\nTotal time taken  = %fs\n", time);
    return 0;
}
