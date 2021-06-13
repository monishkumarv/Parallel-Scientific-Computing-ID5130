#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
#include <omp.h>

#define print_array(a, n)             \
    cout << "Array '" << #a << "': "; \
    for (int i = 0; i < n; i++)       \
        cout << a[i] << " ";          \
    cout << endl;

int main()
{
    double t1, t2;
    long thread_count = 1;
    t1 = omp_get_wtime();

    int n = 2;
    // float a[n][n] = {2, -1, 0, 0, -1, 2, -1, 0, 0, -1, 2, -1, 0, 0, -1, 2};
    // float b[n] = {1, 2, 3, 4};
    // float x[n] = {1, 1, 1, 1};
    // float x_prev[n] = {1, 1, 1, 1};

    float a[n][n] = {1, 1, 1, -1};
    float b[n] = {10, 6};
    float x[n] = {0, 0};
    float x_prev[n] = {0, 0};

    double residual = 1000;
    while (residual > pow(10, -5))
    {

        double sum = 0;
        residual = 0.0;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (j != i)
                {
                    sum += a[i][j] * x_prev[j];
                }
            }

            x[i] = (b[i] - sum) / a[i][i];

            residual += (x[i] - x_prev[i]);
            x_prev[i] = x[i];
        }
    }
    t2 = omp_get_wtime();

    print_array(x, n);

    cout << "\nTime of execution: " << t2 - t1 << endl;

    return 0;
}
